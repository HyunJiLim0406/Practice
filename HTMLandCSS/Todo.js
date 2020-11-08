const toDoForm = document.querySelector(".toDoForm-js")
const toDoInput = toDoForm.querySelector("input");
const toDoList = document.querySelector("toDoList-js");

const TODO_LS = "toDos";
let toDoArr=[];
const newId = toDoArr.length+1;

function saveTodos(){
    localStorage.setItem(TOTO_LS, JSON.stringify(toDoArr));
}

function deleteToDo(event){
    const btn = evnet.target;
    const li = btn.parentNode;
    toDoList.removeChild(li);

    const cleanToDoArr = toDoArr.filter(function(toDo){
        return toDo.id != li.id;
    });
}

const toDo_object = {
    text : text, id : toDoArr.length+1
};
toDoArr.push(toDo_object);
saveTodos();

function paintTodo(text){
    const li = document.createElement("li");
    const delBtn = document.createElement("button");
    delBtn.innerHTML = "❌";
    const span = document.createElement("span");
    const newId = toDoArr.length+1;
    delBtn.addEventListener("click", deleteToDo);

    span.innerText = text;
    li.appendChild(span);
    li.appendChild(delBtn);
    toDoList.appendChild(li);
    li.id = newId;
    const toDo_object={
        text : text, id : newId
    };

    toDoArr.push(toDo_object);
    saveTodos();
}

function handleSubmit(event){
    event.preventDefault();
    const currentValue = toDoInput.value;
    paintTodo(currentValue);
    toDoInput.value="";
}

function loadToDos(){
    const toDos_loaded = localStorage.getItem(TODO_LS);
    if(toDos_loaded !== null){
        const toDos_parsed = JSON.parse(toDos_loaded);
        console.log(toDos_loaded);
        console.log(toDos_parsed);
        for(let i=0;i<toDos_parsed.length;i++){
            paintTodo(toDos_parsed[i].text);
        }
    }
    else{

    }
}

function init(){
    loadToDos();
    toDoForm.addEventListener("submit", handleSubmit);
}

init();