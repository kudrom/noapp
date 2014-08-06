#!/usr/bin/env python2
import audioop, math, shutil, os

def sqsum(data):
    return sum(val * val for val in data)

if __name__ == "__main__":
    if os.path.exists("/tmp/threshold.pcm"):
        shutil.copyfile("/tmp/threshold.pcm", "./threshold.pcm")
    if os.path.exists("./threshold.pcm"):
        filename = "./threshold.pcm"
        with open(filename, 'rb') as f:
            d = f.read()

        audioop_result = audioop.rms(d, 2)
        print("audioop:")
        print("    power: %d" % audioop_result)

        view = memoryview(d)
        formatted = view.cast('h')
        s = sqsum(formatted)
        view_result = math.sqrt(s / len(formatted))
        print( "View:")
        print( "    power: %f" % view_result)
        print( "    size: %d" % len(formatted))
        print( "    sum: %d" % s)

        fft = []
        size = int(len(d) / 2)
        for i in range(0, size):
            fft.append((d[2*i+1] << 8 | d[2*i]))
        s = sqsum(fft)
        custom_result = math.sqrt(s / len(fft))
        print( "Custom:")
        print( "    power: %f" % custom_result)
        print( "    size: %d" % len(fft))
        print( "    sum: %d" % s)

        z = zip(formatted, fft)
        diff = []
        i = 0
        for item in z:
            if item[0] != item[1]:
                j, k = item
                diff.append((i, j, k))
            i += 1
        print( "\nDifferences in %d bytes" % len(diff))
        for i in range(len(diff)-10, len(diff)):
            print( "    Example in byte %d -> view: %d custom: %d" % (diff[i][0], diff[i][1], diff[i][2]))
