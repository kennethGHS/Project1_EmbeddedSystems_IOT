const light_pins = {
    "living-room-light": "pin 1",
    "kitchen-light": "pin 2",
    "dinning-room-light": "pin 3",
    "room1-light": "pin 4",
    "room2-light": "pin 5"
}

const light_states = {
    "living-room-light": "0",
    "kitchen-light": "0",
    "dinning-room-light": "0",
    "room1-light": "0",
    "room2-light": "0"
}

const lights_url = "http://localhost:8000/api/lights";
const lights_ON = "yellow";
const lights_OFF = "black";

function update_light_state() {
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'text';
    xhr.open("GET", lights_url, true);
    xhr.onreadystatechange = function () {
        // Request finished. Do processing here.
        const response = JSON.parse(xhr.responseText)
        for (const light_id in light_pins) {
            const pin = light_pins[light_id];
            light_states[light_id] = response[pin]
        }
        update_lights();
    };
    xhr.send(null);
}

function update_lights() {
    for (const light_id in light_states) {
        const light = document.getElementById(light_id);
        console.log(JSON.stringify(light_states));
        if (light_states[light_id] === "1") {
            light.style.backgroundColor = lights_ON;
        } else {
            light.style.backgroundColor = lights_OFF;
        }
    }
}

document.addEventListener('DOMContentLoaded', (event) => {
    update_light_state();
})

