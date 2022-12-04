'use strict';

const analyzer = require('./mgxnode');
const { resolve } = require('path');

//const absolute_path = resolve('/Workspace/mgxnode/libs/MgxParser/test/test_records/AOC10_4v4_3_192a8268.zip');
const absolute_path = resolve('/Workspace/mgxhub/tmpsave/837b5135760be3309273fa5ecc680b63.zip');
const rec = new analyzer({
    input: absolute_path,
    map: true,
    hd: true,
    mapWidth: 600,
    mapHeight: 300,
    //unzip: 'buffer',
    md5: false
    // header: 'testheader.dat',
    // body: 'testbody.dat',
});

const rec_info = rec.result();
const rec_map_base64 = rec.map_base64();
//rec.savemap('testmap.png');
//rec.unzipsave()

console.log(rec_info);
//console.log(rec_map_base64);