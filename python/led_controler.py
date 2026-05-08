import machine, asyncio
import neopixel
import chip
import file

WHITE = (255, 255, 255)

async def controller():
  np = neopixel.NeoPixel(chip.D5, file.DATA.n_leds)
  
  while True:
    color = await file.DATA.get_color()
    if chip.D2.value():
      color = WHITE

    # Read value from Pot
    val = chip.A0.read_u16()
    percentage = 1 - ((val - 319) / 65535)
  
    # Adjust the brighness of the LEDs by percentage from the Pot
    color = tuple([int(i * percentage) for i in color])

    np.fill(color)
    np.write()
 
    await asyncio.sleep(.1)