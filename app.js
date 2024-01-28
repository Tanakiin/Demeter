const menu = document.querySelector('#mobile-menu');
const menuLinks = document.querySelector('.navbar__menu');

menu.addEventListener('click', function() {
  menu.classList.toggle('is-active');
  menuLinks.classList.toggle('active');
});

function updateConditionalStatements(temp, humidity, lightLvl) {
    let message = '';

    if (temp > 30) {
        message += 'Warning: High temperature! ';
    } else if (temp < 10) {
        message += 'Alert: Low temperature! ';
    }

    if (humidity > 50) {
        message += 'Note: High humidity. ';
    } else if (humidity < 20) {
        message += 'Note: Low humidity. ';
    }

    if (lightLvl < 10) {
        message += 'Reminder: Low light level.';
    }

    document.getElementById('.conditional-statements').innerHTML = message;
}

function fetchData() {
    fetch('http://127.0.0.1:5000/sensor_data')
      .then(response => {
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.json();
      })
      .then(data => {
        console.log('Data received:', data);  // For debugging
        formatSensorData(data);
        

        const temp = parseFloat(data.data.split(', ')[0].split(': ')[1]);
        const humidity = parseFloat(data.data.split(', ')[1].split(': ')[1]);
        const lightlvl = parseFloat(data.data.split(', ')[2].split(': ')[1]);
        updateConditionalStatements(temp, humidity, lightlvl)

      })
      .catch(error => console.error('Error:', error));
}

function formatSensorData(data) {
    // Assuming data comes in the format {"data":"Temp: 21.84 C, Humidity: 31.14%, LightLvl: 35"}
    const sensorValues = data.data.split(', '); // Split the string into an array of sensor readings
    const formattedSensorData = sensorValues.join('<br>'); // Join the array elements with a line break
    document.getElementById('sensorData').innerHTML = formattedSensorData; // Use innerHTML to interpret line breaks
}


// Adjust interval to 1000ms (1 second)
setInterval(fetchData, 1000);