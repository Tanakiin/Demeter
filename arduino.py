from flask import Flask, jsonify
from flask_cors import CORS
import serial

app = Flask(__name__)
CORS(app)
arduino = serial.Serial('/dev/tty.usbserial-0001', 115200, timeout=1)

@app.route('/sensor_data')
def sensor_data():
    data = None
    while arduino.inWaiting() > 0:
        line = arduino.readline().decode('utf-8').strip()
        print(f"Line read from Arduino: {line}")  # Debug print
        if line:  # Check if line is not empty
            data = line
    
    if data:
        print(f"Sending data: {data}")  # Debug print
        return jsonify({'data': data})
    else:
        print("No data available")  # Debug print
        return jsonify({'error': 'No data'})

if __name__ == '__main__':
    app.run(debug=True)
