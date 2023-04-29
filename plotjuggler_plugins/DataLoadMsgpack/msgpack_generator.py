#!/usr/bin/python
# python3 -m pip install msgpack

import msgpack

obj = {
    "timestamp" : 0,
    "right": {
        "arm" : {
            "q" : [0, 1, 2, 3, 4, 5, 6],
            "qd" : [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
        },
        "hand" : {
            "q" : 10,
            "qd" : 100
        }
    },
    "left": {
        "arm" : {
            "q" : [10, 11, 12, 13, 14, 15],
            "qd" : [1.0, 1.1, 1.2, 1.3, 1.4, 1.5]
        },
        "hand" : {
            "q" : 10,
            "qd" : 100
        }
    },
}

with open('msg.mpac', 'wb') as fd:
    for i in range(10):
        obj["timestamp"] = i
        obj["right"]["arm"]["q"][0] = i
        obj["right"]["hand"]["q"] = i * i
        obj["left"]["arm"]["qd"][0] = i * 0.1
        obj["left"]["hand"]["q"] = i * i * 0.1
        fd.write(msgpack.packb(obj))

for msg in msgpack.Unpacker(open('msg.mpac', 'rb')):
    print(msg)