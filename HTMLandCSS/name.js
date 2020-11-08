const form = document.querySelector(".form-js");
const input = form.querySelector("input");
const welcome = document.querySelector(".welcome-js");

const USER_LS = "currentUser";
const SHOWING_CN = "showing";

function saveName(text){
    localStorage.setItem(USER_LS, text);
}

function paintWelcome(text){
    form.classList.remove(SHOWING_CN);
    welcome.classList.add(SHOWING_CN);
    welcome.innerText = `hello ${text}`;
}

function handleSubmit(event){
    event.preventDefault();
    const currentValue = input.value;
    paintWelcome(currentValue);
    saveName(currentValue);
}

function askForName(){
    form.classList.add(SHOWING_CN);
    form.addEventListener("submit", handleSubmit);
}

function loadName(){
    const currentUser = localStorage.getItem(USER_LS);
    if(currentUser===null){
        askForName();
    }
    else{
        paintWelcome(currentUser);
    }
}

function init(){
    loadName();
}

init();