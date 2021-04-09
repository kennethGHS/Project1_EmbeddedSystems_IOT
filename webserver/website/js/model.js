const light_pins = {
    "living-room-light": "pin 0",
    "kitchen-light": "pin 1",
    "dinning-room-light": "pin 2",
    "red-room-light": "pin 3",
    "blue-room-light": "pin 4"
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


const sensor_pins = {
    "front-door-sensor": "pin 0",
    "back-door-sensor": "pin 1",
    "red-room-door-sensor": "pin 2",
    "blue-room-door-sensor": "pin 3"
}

const sensor_states = {}

const sensor_ON = "forestgreen";
const sensor_OFF = "red";

var picture_updated;

function get_server_sensors_state() {
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'text';
    xhr.open("GET", sensors_url, true);
    xhr.onreadystatechange = function () {
        // Request finished. Do processing here.
        const response = JSON.parse(xhr.responseText)
        for (const sensor_id in sensor_pins) {
            const pin = sensor_pins[sensor_id];
            sensor_states[sensor_id] = response[pin]
        }
        update_sensors();
    };
    xhr.send(null);
}

function update_sensors() {
    for (const sensor_id in sensor_states) {
        const sensor = document.getElementById(sensor_id);
        if (sensor_states[sensor_id] === "1") {
            sensor.style.color = sensor_ON;
            sensor.textContent = "OPEN";
        } else {
            sensor.style.color = sensor_OFF;
            sensor.textContent = "CLOSED";
        }
    }
}


function take_picture() {
    const xhr = new XMLHttpRequest();
    picture_updated = false;
    xhr.responseType = 'json';
    xhr.open("POST", take_picture_url, true);
    xhr.onreadystatechange = function () {
        var picture_timer = setInterval(show_picture, 1000);
        setTimeout(() => {
            clearInterval(picture_timer);
        }, 5000);

        function show_picture() {
            const xhr = new XMLHttpRequest();
            xhr.responseType = 'text';
            xhr.open("GET", take_picture_url, true);
            xhr.onreadystatechange = function () {
                // Request finished. Do processing here.
                const response = JSON.parse(xhr.responseText)
                if (response["photo"] === 0 && !picture_updated) {
                    picture_updated = true;
                    clearInterval(picture_timer);
                    show_modal();
                }
            }
            xhr.send(null);
        }
    }
    xhr.send(JSON.stringify({"photo": 1}));
}


function hide_modal() {
    var modal = document.getElementById("myModal");
    modal.style.display = "none";
}

function show_modal() {
    var modal = document.getElementById("myModal");
    var modalImg = document.getElementById("picture");
    modal.style.display = "block";
    modalImg.src = get_picture_url + "?t=" + new Date().getTime();
}

document.addEventListener('DOMContentLoaded', (event) => {
    validate_session(model_url, login_url);
    get_server_light_state();
    get_server_sensors_state();
    setInterval(get_server_sensors_state, 5000);
})
