'use strict';

const mgxnode = require('./build/Release/mgxnode.node');
const fs = require('fs');

function RecordAnalyzer(input) {
    this._raw = mgxnode.parse(input);
    this._result_cache;
    this.result = () => {
        if (this._result_cache) {
            return this._result_cache;
        } else {
            return (this._result_cache = JSON.parse(this._raw ? this._raw.result : '{}'));
        }
    };
    this.map_base64 = () => {
        if (this._raw.mapbuffer) return this._raw.mapbuffer.toString('base64');
    };
    this.savemap = (path) => {
        fs.writeFile(path, this._raw ? this._raw.mapbuffer ?? '' : '', function (err) {
            if (err) {
                return console.log(err);
            }
        });
    };
}

module.exports = RecordAnalyzer;
