import time, uuid

def generate_timeuuid():
    timestamp = int(time.time() * 1000)  # milliseconds since epoch
    interval = timestamp * 10000 + 0x01b21dd213814000
    timeuuid = uuid.UUID(int=interval, version=1)
    return timeuuid