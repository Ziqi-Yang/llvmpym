from llvmpym import core

if __name__ == '__main__':
    a = core.Module("hi")
    a.id = "Hello"
    print(a.id)

