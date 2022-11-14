'use strict';

console.time('Loop oldrecs');

const fs = require('fs');
const analyzer = require('./mgxnode');
const mgxnode = require('./build/Release/mgxnode.node');
const path = require('node:path');
const opendir_sync = require('node:fs').opendirSync;

var cnt = 0;
var flaw = 0;
var limit = 0; // 0 = unlimited
async function loopOver(d) {
    const dir = opendir_sync(d, { bufferSize: 200 });
    for await (const entry of dir) {
        if (entry.isDirectory()) {
            await loopOver(path.join(d, entry.name));
        } else {
            process.stdout.write(`#${cnt} ${path.join(d, entry.name)} | `);
            let rec = new analyzer({
                input: path.join(d, entry.name),
            });
            //rec.result().map_base64();

            console.warn(`Ver:${rec.result().version ? rec.result().version.code : 'unknown'}. Status:${rec.result().status}.`);
            if (rec.result().message || rec.result().status != 'perfect') {
                try {
                    fs.writeFileSync(`notgood-${rec.result().status}.txt`, path.join(d, entry.name) + '\n', {
                        flag: 'a',
                    });
                    fs.writeFileSync(
                        `notgood-${rec.result().status}-msg.txt`,
                        path.join(d, entry.name) + ` ${rec.result().message}` + '\n',
                        { flag: 'a' }
                    );
                    // file written successfully
                } catch (err) {
                    console.error(err);
                }
            }
            if (rec.result().message) flaw++;
            cnt++;
        }
        if (limit > 0 && cnt >= limit) break;
    }
}

async function main() {
    await loopOver('/Oldrecs');

    return 'done.';
}

main().then(() => process.exit());

process.on('exit', () => {
    console.timeEnd('Loop oldrecs');
    const used = process.memoryUsage().heapUsed / 1024 / 1024;
    console.log(`${cnt} records reviewed, ${flaw} are not good, uses approximately ${used} MB\n`);
});
