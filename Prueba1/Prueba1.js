/*
 * @name Setup and Draw
 * @description The code inside the draw() function runs continuously from top
 * to bottom until the program is stopped.
 */

 var serial;
 var latestData = [];  // you'll use this to write incoming data to the canvas

var maxGrados = 50;
var minGrados = -60;

var setPoint;

var estadoP;
var estadoI;
var estadoD;

var controlUsado;


/* #################################################### */
/* #################################################### */
//Funcion para cargar la grafica. Debe de ir afuera de todo
window.onload = function () {

    var dps = []; // dataPoints
    var dps2 = [];

    var errores = [];
    var manipulaciones = [];

    var chart = new CanvasJS.Chart("chartContainer",{

      data: [{
        type: "line",
        showInLegend: true,
        name: "series1",
        legendText: "Set Point",
        dataPoints: dps
      },{
        type: "line",
        showInLegend: true,
        name: "series2",
        legendText: "Medición",
        dataPoints: dps2
      }],
      axisY:{
        interval: 10,
        viewportMinimum: -60,
        viewportMaximum: 60
      }
    });
    var chart2 = new CanvasJS.Chart("chartContainer2",{

      data: [{
        type: "line",
        showInLegend: true,
        name: "series1",
        legendText: "Error",
        dataPoints: errores
      },{
        type: "line",
        showInLegend: true,
        name: "series2",
        legendText: "Manipulación",
        dataPoints: manipulaciones
      }],
      axisY:{
        interval: 10,
        viewportMinimum: -60,
        viewportMaximum: 90
      }
    });

    var xVal = 0;
    var yVal;
    var updateInterval = 50;
    var dataLength = 500; // number of dataPoints visible at any point

    var updateChart = function (count) {
      count = count || 1;
      // count is number of times loop runs to generate random dataPoints.

      for (var j = 0; j < count; j++) {
        yVal = setPoint;//yVal +  Math.round(5 + Math.random() *(-5-5));
        dps.push({
          x: xVal,
          y: yVal
        });
        dps2.push({
          x: xVal,
          y: parseFloat(latestData[0])-60
        });
        errores.push({
          x: xVal,
          y: parseFloat(latestData[1])
        });
        manipulaciones.push({
          x: xVal,
          y: parseFloat(latestData[2])
        })
        xVal++;
      };
      if (dps.length > dataLength)
      {
        dps.shift();
        dps2.shift();
        errores.shift();
        manipulaciones.shift();
      }

      chart.render();
      chart2.render();

    };

    // generates first set of dataPoints
    updateChart(dataLength);

    // update chart after specified time.
    setInterval(function(){updateChart()}, updateInterval);

  }



  /* #################################################### */
  /* #################################################### */
function setup() {


  /* #################################################### */
  /* #################################################### */
    createCanvas(0, 0);
    serial = new p5.SerialPort();

    serial.open("/dev/ttyACM0");
    serial.on('data', gotData);

    /* #################################################### */
    /* #################################################### */

  document.getElementById("controladorActual").innerHTML = "PID";
  setPoint = parseInt(document.getElementById("setPoint").value);


  document.getElementById("valorPasado").innerHTML = setPoint;

  $('#enterSetPoint').button();
  $('#enterSetPoint').click(function() {
    document.getElementById("valorPasado").innerHTML = setPoint;
    setPoint = parseInt(document.getElementById("setPoint").value);
    if (setPoint > maxGrados) {
      setPoint = maxGrados;
    }
    if (setPoint < minGrados) {
      setPoint = minGrados;
    }
    document.getElementById("setPoint").value = setPoint;
    serial.write(controlUsado + "," + setPoint.toString());
});
/* #################################################### */
/* #################################################### */    /* #################################################### */
    /* #################################################### */
    $(document).on('change', '#controlador,#P,#I,#D', function (event) {
      controlUsado = $('#controlador label.active input').val();
      estadoP = document.getElementById("P").checked;
      estadoI = document.getElementById("I").checked;
      estadoD = document.getElementById("D").checked;

      if (controlUsado === "PID"){
        var temp = controlUsado.split("");
        if (estadoP == false){
          temp.splice(0,1);
        }
        if (estadoD == false) {
          temp.splice(temp.indexOf("D"),1);
        }
        if (estadoI == false) {
          temp.splice(temp.indexOf("I"),1);
        }
        controlUsado = temp.join("");
      }
      document.getElementById("controladorActual").innerHTML = controlUsado;

      //serial.write(controlUsado + "," + setPoint.toString());

      if (controlUsado === "PID"){
        controlUsado = 1;
      }
      else if (controlUsado === "PI") {
        controlUsado = 2;
      }
      else if (controlUsado === "P") {
        controlUsado = 3;
      }
      else if (controlUsado === "PD") {
        controlUsado = 4;
      }
      else if (controlUsado === "ID") {
        controlUsado = 5;
      }
      else if (controlUsado === "I") {
        controlUsado = 6;
      }
      else if (controlUsado === "D") {
        controlUsado = 7;
      }
      else if (controlUsado === "Dahlin") {
        controlUsado = 8;
      }

      serial.write(controlUsado + "," + setPoint.toString());

      });
      /* #################################################### */
      /* #################################################### */
      setTimeout(function(){serial.write("PID" + "," + minGrados.toString()); }, 2000);

}
/* #################################################### */

function gotData() {
  var currentString = serial.readLine();  // read the incoming string
  trim(currentString);                    // remove any trailing whitespace
  if (!currentString) return;             // if the string is empty, do no more
  console.log(currentString);             // println the string

  latestData = currentString.split(",");
}

/* #################################################### */
/* #################################################### */

function sendData(){
  console.log("hola");
}

function draw() {

  document.getElementById("medicion").innerHTML = parseFloat(latestData[0])-60;
  document.getElementById("error").innerHTML = parseFloat(latestData[1]);
  document.getElementById("manipulacion").innerHTML = parseFloat(latestData[2]);
}
