window.addEventListener('DOMContentLoaded', (event) => {
    console.log('DOM fully loaded and parsed');
    const form = document.getElementById('login-form');
    form.addEventListener('submit', event => {
        // submit event detected
        let email = document.getElementById('input-email');
        email = CryptoJS.SHA256(email.value).toString();

        let password = document.getElementById('input-password');
        password = CryptoJS.SHA256(password.value).toString();
        login(email, password, auth_url);

        validate_session(model_url, login_url)
        event.preventDefault();

    })
});


function sign_up() {
    let email = document.getElementById('input-email');
    email = CryptoJS.SHA256(email.value).toString();

    let password = document.getElementById('input-password');
    password = CryptoJS.SHA256(password.value).toString();
    login(email, password, sign_url);

    validate_session(model_url, sign_url);

}

function login(email, password, url) {

    const xhr = new XMLHttpRequest();
    xhr.open("POST", url, false);
    xhr.onreadystatechange = function () {
        token = xhr.getResponseHeader("Authorization").toString();
        if (token === "0") {
            alert("Username or password are incorrect");
        }
        localStorage.setItem("token", token);
    }
    xhr.send(JSON.stringify({username: email, password: password}));
}

function validate_session(next_url, default_url) {
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'text';

    xhr.open("POST", session_url, true);
    xhr.onreadystatechange = function () {
        const response = JSON.parse(xhr.responseText)

        if (response["token"] === "valid") {
            if (window.location.href !== next_url) {
                window.location = next_url;
            }
        } else if (window.location.href !== default_url) {

            window.location = default_url;
        }

    }
    xhr.send(JSON.stringify({"token": localStorage.getItem('token').toString()}));
}