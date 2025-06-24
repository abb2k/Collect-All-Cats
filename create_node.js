// create_node.js
const fs = require('fs');
const path = require('path');

const name = process.argv[2];
if (!name) {
    console.error("Usage: node create_node.js <NodeName>");
    process.exit(1);
}

const hppName = `${name}.hpp`;
const cppName = `${name}.cpp`;

const hppContent = `#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ${name} : public CCNode {
public:
    static ${name}* create();

private:
    virtual bool init();
};
`;

const cppContent = `#include "${hppName}"

${name}* ${name}::create() {
    auto ret = new ${name}();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ${name}::init() {
    if (!CCNode::init()) return false;

    return true;
}
`;

fs.writeFileSync(path.join(process.cwd(), hppName), hppContent);
fs.writeFileSync(path.join(process.cwd(), cppName), cppContent);

console.log(`Created ${hppName} and ${cppName}`);
