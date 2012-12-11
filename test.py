
class C:
    x = 2
    def __init__(self, x):
        self.x = x
    def f(self):
        return x
o = C(3)
print x
print C.x
print C.f(o)
print o.x
print o.f()
