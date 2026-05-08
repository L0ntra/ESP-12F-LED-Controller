#### WEB Example
import network
import file
import ujson
import gc

def connect_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        pass
    print('Connected! IP:', wlan.ifconfig()[0])

async def home_handler(reader, writer, content_length):
    current_rgb = await file.DATA.get_color()
    current_hex = rgb_to_hex(current_rgb)
    
    print(f"Loading page with current color: {current_hex}")

    # 2. Inject that hex string into the 'value' attribute of the input
    html_form = f"""<!DOCTYPE html>
    <html>
    <head><title>ESP8266 LED Controller</title></head>
    <body>
        <h2>Color Picker</h2>
        
        <input type="color" id="colorPicker" value="{current_hex}">
        
        <button onclick="sendJson()">Send</button>
        <script>
            async function sendJson() {{
                const colorVal = document.getElementById('colorPicker').value;
                const res = await fetch('/set-color', {{
                    method: 'POST',
                    headers: {{ 'Content-Type': 'application/json' }},
                    body: JSON.stringify({{ color: colorVal }})
                }});
            }}
        </script>
    </body>
    </html>"""
    
    response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n" + html_form
    writer.write(response.encode('utf-8'))
    await writer.drain()    

async def set_color_handler(reader, writer, content_length):
    if content_length <= 0:
        writer.write(b"HTTP/1.1 411 Length Required\r\nConnection: close\r\n\r\n")
        await writer.drain()
        return

    try:
        body = await reader.readexactly(content_length)
        data = ujson.loads(body)
        hex_color = data.get('color', '#000000')
        rgb = hex_to_rgb(hex_color)
        
        await file.DATA.set_color(rgb)
        await file.write()
        
        response = f"HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nRGB: {rgb}"
        writer.write(response.encode('utf-8'))
    except Exception as e:
        print(f"POST Error: {e}")
        writer.write(b"HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n")
    
    await writer.drain()    

async def not_found_handler(reader, writer, content_length):
    writer.write(b"HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n")
    await writer.drain()

ROUTES = {
    ('GET', '/'): home_handler,
    ('POST', '/set-color'): set_color_handler
}

async def handle_client(reader, writer):   
    try:
        # Read the request line (e.g., "GET / HTTP/1.1")
        line = await reader.readline()
        if not line:
            return
        
        request_line = line.decode('utf-8').split()
        if len(request_line) < 2:
            return
            
        method, path = request_line[0], request_line[1]

        # Read headers to find Content-Length and clear the buffer
        content_length = 0
        while True:
            line = await reader.readline()
            if line == b'\r\n' or line == b'\n' or line == b'':
                break
            if line.lower().startswith(b'content-length:'):
                content_length = int(line.split(b':')[1].strip())

        handler = ROUTES.get((method, path), not_found_handler)
        
        print(f"Routing {method} {path} to {handler.__name__}")
        await handler(reader, writer, content_length)

    except Exception as e:
        print(f"Error handling request: {e}")
    finally:
        await writer.wait_closed()
        print("--- Connection Closed ---\n")
        gc.collect() # Crucial for ESP8266 RAM management

def hex_to_rgb(hex_str):
    # Remove the '#' if it's there
    hex_str = hex_str.lstrip('#')
    
    # Slice the string into 2-character chunks and convert from hex (base 16) to int
    r = int(hex_str[0:2], 16)
    g = int(hex_str[2:4], 16)
    b = int(hex_str[4:6], 16)
    
    return (r, g, b)

def rgb_to_hex(rgb_tuple):
    print(rgb_tuple)
    # %02x formats the integer as a 2-character, zero-padded hexadecimal string
    return "#{:02x}{:02x}{:02x}".format(rgb_tuple[0], rgb_tuple[1], rgb_tuple[2])
