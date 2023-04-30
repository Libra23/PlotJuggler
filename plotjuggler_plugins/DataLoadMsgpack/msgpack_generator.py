#!/usr/bin/python
# python3 -m pip install msgpack

import msgpack
import math

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
    mm = 10
    for i in range(mm * 60 * 1000):
        obj["timestamp"] = i * 0.0001
        obj["right"]["arm"]["q"][0] = math.sin(i * 0.0001)
        obj["right"]["hand"]["q"] = i * i
        obj["left"]["arm"]["qd"][0] = math.sin(i * i * 0.0001)
        obj["left"]["hand"]["q"] = i * i * i
        fd.write(msgpack.packb(obj))