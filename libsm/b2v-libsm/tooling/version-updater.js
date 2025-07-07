#!/usr/bin/env node
const fs = require('fs');

module.exports.readVersion = function (contents) {
    let version;
    try {
        let matches = contents.match(/project\(.*VERSION (\d+\.\d+\.\d+)\)/);
        version = matches[1];
    } catch (e) {
        if(e.message === "Cannot read property '1' of null") {
            throw new Error("couldn't read version from file");
        }
        throw e;
    }
    //console.log("read version: " + version);
    return version;
}

module.exports.writeVersion = function (contents, version) {

    contents = contents.replace(/project\((.*) VERSION \d+\.\d+\.\d+\)/, `project($1 VERSION ${version})`);
    //console.log(contents);

    let writtenVersion = module.exports.readVersion(contents);
    if(version != writtenVersion) {
        throw new Error(`issue updating version, given '${version}', tried to save it as '${writtenVersion}'`);
    }
    return contents;
}

if (process.mainModule.filename === __filename) {
    let contents = fs.readFileSync('CMakeLists.txt', 'utf8');
    let verCur = module.exports.readVersion(contents);
    newContents = module.exports.writeVersion(contents, '5.2.1');
    let verNew = module.exports.readVersion(newContents);
    if(verCur === verNew) {
        throw new Error("failed to update version");
    }
    console.log(`updated version from ${verCur} to ${verNew}`);
}
