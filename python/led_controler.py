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
    # The Pot is wired in the opposite direction than intended. Instead of resoldering, I 
    # decided to subtract the ratio from 1 to maintain a natural feel on the dial.
    # The Pot also has a dead zone under 319 wich was restricting the LEDs from reaching their
    # maximum brightness. To fix that we subtract 319 from the value which enables the ratio to
    # 0, but not 1. Not reaching 1 means the Pot cannot make the LEDs turn off, which is fine
    # since the toggle switch controls turning the LED strip all the way off.
    val = chip.A0.read_u16()
    percentage = 1 - ((val - 319) / 65535)
  
    # Adjust the brighness of the LEDs by percentage from the Pot
    color = tuple([int(i * percentage) for i in color])

    np.fill(color)
    np.write()
 
    await asyncio.sleep(.1)
