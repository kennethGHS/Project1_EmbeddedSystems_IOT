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

function light_state() {
    console.log(JSON.stringify(light_states));
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'text';
    xhr.open("GET", lights_url, true);
    xhr.onreadystatechange = function () {
        // Request finished. Do processing here.
        const response = JSON.parse(xhr.responseText)
        for (const light in light_pins) {
            const pin = light_pins[light];
            light_states[light] = response[pin]
        }
        console.log(JSON.stringify(light_states));

    };
    xhr.send(null);
}

light_state();