
function mouseOverAction(event){
    var element = event.currentTarget;
    var link = element.querySelector("a");
    element.className = "list-group-item active";
    link.style.color="#66e0ff";
    element.style.fontWeight="bold";

}
function mouseLeaveAction(event){
    var element = event.currentTarget;
    var link = element.querySelector("a");
    element.className = "list-group-item";
    link.style.color="white";
    element.style.fontWeight="normal";

}

function thankYouMsg(event){
    alert("Thank you for your time.");
}
