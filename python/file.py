import json, asyncio

FILE_NAME = 'data.json'

color_lock = asyncio.Lock()
class DataObject:
  color = (0, 0, 0)
  def __init__(self, dict):
    for k, v in dict.items():
      setattr(self, k, v)
          
  async def get_color(self):
    async with color_lock:
      return tuple(self.color)
          
  async def set_color(self, c):
    print(c)
    async with color_lock:
      self.color = c
      

DATA = None

def read():
  with open(FILE_NAME, 'r') as f:
      global DATA
      DATA = DataObject(json.load(f))
      print(DATA.color)
      
wlock = asyncio.Lock()
async def write():
  async with wlock:
    # global DATA
    # DATA.color = color

    with open(FILE_NAME, 'w') as f:
      json.dump(DATA.__dict__, f)
    