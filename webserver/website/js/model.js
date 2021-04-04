const light_pins = {
    "living-room-light": "pin 1",
    "kitchen-light": "pin 2",
    "dinning-room-light": "pin 3",
    "red-room-light": "pin 4",
    "blue-room-light": "pin 5"
}

const light_states = {}

const lights_ON = "#F8F272";
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
        light_states[light_id] = light_states[light_id] === 1 ? 0 : 1;
        update_lights();
        set_server_light_state();
    }
}

function switch_all_lights(light_id) {
    if (light_id in light_states) {
        const state = light_states[light_id] === 1 ? 0 : 1;

        for (const light_id in light_states) {
            light_states[light_id] = state;
        }
        update_lights();
        set_server_light_state();
    }
}

function update_lights() {
    for (const light_id in light_states) {
        const light = document.getElementById(light_id);
        const light_label = document.getElementById(light_id + "-label");
        console.log(JSON.stringify(light_states));
        if (light_states[light_id] === 1) {
            light.style.backgroundColor = lights_ON;
            light_label.style.color = lights_ON;
            light_label.textContent = "ON";
        } else {
            light.style.backgroundColor = lights_OFF;
            light_label.style.color = lights_OFF;
            light_label.textContent = "OFF";
        }
    }
}


document.addEventListener('DOMContentLoaded', (event) => {
    validate_session(model_url, login_url);
    get_server_light_state();
})

