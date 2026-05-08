# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import os, asyncio
#os.dupterm(None, 1) # disable REPL on UART(0)
import gc
#import webrepl
#webrepl.start()
gc.collect()

import web_server as web
import led_controler as led
import file

file.read()

# 3. Main async loop
async def main():
    web.connect_wifi(file.DATA.ssid, file.DATA.password)
    
    # Start the server on port 80
    _led_task = asyncio.create_task(led.controller())
    
    _web_server = await asyncio.start_server(web.handle_client, "0.0.0.0", 80)
    print("Server started...")
        
    # The server runs as a background task; keep the loop alive
    while True:
        await asyncio.sleep(1)

try:
    asyncio.run(main())
except KeyboardInterrupt:
    print("Server stopped")