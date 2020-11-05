//object literal

const book1 = {
    title : '우리가 빛의 속도로 갈 수 없다면',
    author : '김초엽',
    year : '2019',
    getSummary : function(){
        return `<${this.title}>은 ${this.author} 작가의 ${this.year}년도 작품이다.`;
    }
}

const book2 = {
    title : '해리포터',
    author : 'jk rowling',
    year : '1997',
    getSummary : function(){
        return `<${this.title}>은 ${this.author} 작가의 ${this.year}년도 작품이다.`;
    }
}
console.log(book1.title);
console.log(book1.getSummary());
console.log(Object.keys(book1));
console.log(Object.values(book1));
