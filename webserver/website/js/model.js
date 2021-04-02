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

function get_server_light_state() {
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

function set_server_light_state() {
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'json';
    xhr.open("POST", lights_url, true);
    const request = {}
    for (const light_id in light_pins) {
        const pin = light_pins[light_id];
        request[pin] = light_states[light_id]
    }
    xhr.send(JSON.stringify(request));
}

function switch_light_state(light_id) {
    if (light_id in light_states) {
        light_states[light_id] = light_states[light_id] === "1" ? "0" : "1";
        update_lights();
        set_server_light_state();
    }
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
    get_server_light_state();
})

