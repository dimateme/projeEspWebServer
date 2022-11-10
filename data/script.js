
//fonction qui permet de recuperer les données de l'api

function getFromESP_getWoodOptions(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var slelectValue = document.getElementById("liste_bois");
      var data = JSON.parse(this.responseText);
      for (var i = 0; i < data.length; i++) {
        var option = document.createElement("option");
        option.value = data[i].id;
        option.text = data[i].nom;
        slelectValue.appendChild(option);
      }
    }
  }
  xhttp.open("GET", "getAllWoodOptions", true);
  xhttp.send();
}

function getFromESP_getWoodCaracteristiques(){
  var boisId = document.getElementById("liste_bois").value;
  console.log(boisId);
  var params = String("actionBoisId") + String("=") + String(boisId);
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var data = JSON.parse(this.responseText);
      console.log(data);
      document.getElementById("nomBois").innerHTML = data[0].nom;
      document.getElementById("typeBois").innerHTML = data[0].type;
      document.getElementById("origineBois").innerHTML = data[0].origine;
      document.getElementById("tempsBois").innerHTML = data[0].temps;
      document.getElementById("temperatureBois").innerHTML = data[0].temperature;
    }
  }
  xhttp.open("POST", "getCaracteristiqueBois", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(params);
}

//fonction qui permet d'obtenir la température
setInterval(function getFromESP_getNom(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.status == 200) {
        document.getElementById("temp_celcius").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();
},1000);

//fonction qui permet de demarrer le four
function doAction(actionToDo) {
    var params = String("actionToDo") + String("=") + String(actionToDo);
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "DemarrerFour", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhttp.send(params);
}
//la foncion permet de compter l temps de sechage
setInterval(function getCompteur(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.status == 200) {
    document.getElementById("compteurSeconde").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "lireCompteure", true);
    xhttp.send();
   }, 1000);


   var optionsCache = [];

   function filterItems(el) {
     var value = el.value.toLowerCase();
     var form = el.form;
     var opt, sel = form.liste_bois;
     if (value == '') {
       restoreOptions();
     } else {
       // Loop backwards through options as removing them modifies the next
       // to be visited if go forwards
       for (var i=sel.options.length-1; i>=0; i--) {
         opt = sel.options[i];
         if (opt.text.toLowerCase().indexOf(value) == -1){
           sel.removeChild(opt)
         }
       }
     }
   }

   // Restore select to original state
function restoreOptions(){
    var sel = document.getElementById('liste_bois');
    sel.options.length = 0;
    for (var i=0, iLen=optionsCache.length; i<iLen; i++) {
      sel.appendChild(optionsCache[i]);
    }
  }
  
  
  window.onload = function() {
    // Load cache
    var sel = document.getElementById('liste_bois');
    for (var i=0, iLen=sel.options.length; i<iLen; i++) {
      optionsCache.push(sel.options[i]);
    }
  }