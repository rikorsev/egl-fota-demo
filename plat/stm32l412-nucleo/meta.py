import os
import argparse
import crcmod

test_data_a=[0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011]
test_data_b=[0x1100FFEE, 0xDDCCBBAA, 0x88776655, 0x44332211]

POLY=0x104c11db7
SIZE_POSITION=4
CRC_POSITION=8

def process_size(args):
    stats = os.stat(args.filename)

    size = stats.st_size - int(args.meta_size)
    print(f'SIZE: {size} ({hex(size)})')
    size = size.to_bytes(4, "little")

    with open(args.filename, 'rb+') as f:

        # Write binary size to the binary minus size of meta block
        f.seek(SIZE_POSITION)
        f.write(size)

def crc_test():
    crc32 = crcmod.mkCrcFun(POLY, initCrc=0xFFFFFFFF, xorOut=0)

    test_bytes_a = bytes()
    for val in test_data_a:
        test_bytes_a += val.to_bytes(4, "big")

    test_bytes_b = bytes()
    for val in test_data_b:
        test_bytes_b += val.to_bytes(4, "big")

    crc_result = crc32(test_bytes_a)
    crc_result = crc32(test_bytes_b, crc_result)

    print(f'CRC: {crc_result} ({hex(crc_result)})')

def process_crc(args):
    crc32 = crcmod.mkCrcFun(POLY, initCrc=0xFFFFFFFF, xorOut=0)

    with open(args.filename, 'rb+') as f:

        # Ignore meta block
        f.seek(int(args.meta_size))
        data = f.read()

        # Calculate the binary CRC
        crc_val = crc32(data)
        print(f'CRC: {crc_val} ({hex(crc_val)})')
        crc_val = crc_val.to_bytes(4, "little")

        # Write CRC to the binary
        f.seek(CRC_POSITION)
        f.write(crc_val)

def main(args):
    process_size(args)
    process_crc(args)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(
        prog='Meta',
        description='Fulfill binary file with it meta data (size, checksum)'
    )

    parser.add_argument('filename')
    parser.add_argument('meta_size')

    args = parser.parse_args()

    main(args)
