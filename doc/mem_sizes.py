import math

avgq = 16.5
p = 4
i = 4
l = 8
u = 6724891

def lru(c):
    return math.ceil(c * (avgq + 3 * p) / 1024)

def inmem_lfu(c):
    return c * (avgq + 6 * p + i)

def pure():
    return u * (avgq + i)

def pure_lfu(c):
    return inmem_lfu(c) + pure()

def tiny(w, b):
    return math.ceil((6 * w + b * 3 * w) / 8)

def tiny_flu(c, w, b):
    return inmem_lfu(c) + tiny(w, b)

def inmem_gdsf(c):
    return c * (avgq + 6 * p + l + i) + l

def tiny_gdsf(c, w, b):
    return inmem_gdsf(c) - c * i + tiny(w, b)

def pure_gdsf(c):
    return inmem_gdsf(c) - c * i + pure()

def sdc_gdsf_tiny(c, w, b, l):
    return l * c * avgq + (1 - l) * tiny_gdsf(c, w, b)

def sdc_gdsf_pure(c, l):
    return l * c * avgq + (1 - l) * pure_gdsf(c)
