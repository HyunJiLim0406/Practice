let emojiArr = ["🍔","🍖","🍚","🍕","🥗","🍜"];
let foodName = ["햄버거","고기","밥","피자","샐러드","국수"];


const btn = document.querySelector(".btn");
btn.addEventListener("click", 
    function(){
        const index = Math.floor(Math.random() * emojiArr.length);  
        document.querySelector(".em").innerHTML = emojiArr[index];
        console.log(index);
        document.querySelector("h1").innerHTML = `오늘은 ${foodName[index]} 먹자!`;
});