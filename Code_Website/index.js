// Configuration of the API endpoint 
const config = {
   // AWS API Gateway URL to be inserted here
    api_url: 'https://zju159ayv8.execute-api.us-east-1.amazonaws.com/dev'
  };

/* Global Data */

 // Data for flow chart
 var x1_values_fire = [];
 var x2_values_fire = [];
 var x3_values_fire = [];
 var x4_values_fire = [];
/* Statistics */

var min_flame;
var max_flame;
var min_co;
var max_co;
var min_temp;
var max_temp;

// Average measured source water flow
var avg_measured_flame = 0;
var avg_measured_co = 0;
var avg_measured_temp = 0;
var last_date;
var last_date2;
var last_date3;

// Average detected leakage
var avg_leakage = 0;

function dataLoading(parsed_data){

  // Extracting body_flow section of the response
  const fire_data = parsed_data.body_fire;

  // Parsing contents
  const parsed_fire_data = JSON.parse(fire_data);

  // Select last 10 values if there are more than 10
  var start = 0;
  if (parsed_fire_data.length > 10) {
    start = parsed_fire_data.length - 10;
  }

  // Flow data processing
  for (let i = start; i < parsed_fire_data.length; i++) {

    const datetime = parsed_fire_data[i]["Datetime"];
    // Convert sample time to Locale format
    var value = datetime.toLocaleString(); 
    
    // Add value to values_x array
    x1_values_fire.push(value); 

    value = parsed_fire_data[i]["Flame"];
    x2_values_fire.push(value);

    value = parsed_fire_data[i]["CO"];
    x3_values_fire.push(value);

    value = parsed_fire_data[i]["Temp"];
    x4_values_fire.push(value);

  }
}

/* Source water flow statistics */
async function getStatistics() {

  // Reset statistics
  avg_measured_flame = 0;
  avg_measured_co = 0;
  avg_measured_temp = 0;
  len = 0;
  const now = new Date();

  const day = ("0" + now.getDate()).slice(-2);
  const month = ("0" + (now.getMonth() + 1)).slice(-2);
  const year = now.getFullYear();

  let today = day + '/' + month + '/' + year;

  // flame 

  var avg_dict_flame = {};
  var total_flame = 0;

  for (let i = 0; i < x2_values_fire.length; i++) {

    total_flame += parseFloat(x2_values_fire[i]);

    var current_datetime = x1_values_fire[i].toLocaleString().substring(0,15);
    console.log(current_datetime);

    if (today != current_datetime) {
      if (typeof avg_dict_flame[current_datetime] === "undefined") {
        avg_dict_flame[current_datetime] = [parseFloat(x2_values_fire[i])];
      }
      else {
        avg_dict_flame[current_datetime].push(parseFloat(x2_values_fire[i]));
      }
    }

  }

  // co 
  
  var avg_dict_co = {};
  var total_co = 0;

  for (let i = 0; i < x3_values_fire.length; i++) {

    total_co += parseFloat(x3_values_fire[i]);

    var current_datetime = x1_values_fire[i].toLocaleString().substring(0,15);
    console.log(current_datetime);

    if (today != current_datetime) {
      if (typeof avg_dict_co[current_datetime] === "undefined") {
        avg_dict_co[current_datetime] = [parseFloat(x3_values_fire[i])];
      }
      else {
        avg_dict_co[current_datetime].push(parseFloat(x3_values_fire[i]));
      }
    }

  }

  // temp

  var avg_dict_temp = {};
  var total_temp = 0;

  for (let i = 0; i < x4_values_fire.length; i++) {

    total_temp += parseFloat(x4_values_fire[i]);

    var current_datetime = x1_values_fire[i].toLocaleString().substring(0,15);
    console.log(current_datetime);

    if (today != current_datetime) {
      if (typeof avg_dict_temp[current_datetime] === "undefined") {
        avg_dict_temp[current_datetime] = [parseFloat(x4_values_fire[i])];
      }
      else {
        avg_dict_temp[current_datetime].push(parseFloat(x4_values_fire[i]));
      }
    }

  }

  var len = x1_values_fire.length;
  last_date = x1_values_fire[len-1];

  avg_measured_flame = parseFloat(total_flame / x2_values_fire.length);
  avg_measured_co = parseFloat(total_co / x3_values_fire.length);
  avg_measured_temp = parseFloat(total_temp / x4_values_fire.length);

  min_flame = Math.min.apply(null, x2_values_fire);
  max_flame = Math.max.apply(null, x2_values_fire);
  min_co = Math.min.apply(null, x3_values_fire);
  max_co = Math.max.apply(null, x3_values_fire);
  min_temp = Math.min.apply(null, x4_values_fire);
  max_temp = Math.max.apply(null, x4_values_fire);
  last_date2=last_date;
  last_date3=last_date;
}
    
function plot(id, x_values, y_values) {
  var color = "black";

  if (id === "flamechart") {
    color = "blue";
  }
  else if (id === "cochart") {
    color = "red";
  }
  else if (id === "tempchart") {
    color = "green";
  }

  const ctx = document.getElementById(id).getContext('2d');
  new Chart(ctx, {
    type: "bar",
    data: {
      labels: x_values,
      datasets: [{
        backgroundColor: color,
        data: y_values
      }]
    },
    options: {
      legend: {display: false},
      title: {display: false},
      scales: {
        yAxes: [{
            ticks: { beginAtZero: true }
        }]
      }
    }
  });
}


async function display() {
  await callAPI()

  plot("flamechart", x1_values_fire, x2_values_fire);
  plot("cochart", x1_values_fire, x3_values_fire);
  plot("tempchart", x1_values_fire, x4_values_fire);

  getStatistics();
  document.getElementById("avg-flame").innerHTML = Number((avg_measured_flame).toFixed(1)); 
  document.getElementById("min-flame").innerHTML = Number((min_flame).toFixed(1));
  document.getElementById("max-flame").innerHTML = Number((max_flame).toFixed(1));

  document.getElementById("avg-co").innerHTML = Number((avg_measured_co).toFixed(1)); 
  document.getElementById("min-co").innerHTML = Number((min_co).toFixed(1));
  document.getElementById("max-co").innerHTML = Number((max_co).toFixed(1));

  document.getElementById("avg-temp").innerHTML = Number((avg_measured_temp).toFixed(1));
  document.getElementById("min-temp").innerHTML = Number((min_temp).toFixed(1));
  document.getElementById("max-temp").innerHTML = Number((max_temp).toFixed(1)); 

  document.getElementById("last-date").innerHTML = String((last_date)); 
  document.getElementById("last-date2").innerHTML = String((last_date2)); 
  document.getElementById("last-date3").innerHTML = String((last_date3)); 
}


async function callAPI() {

  // Instantiate a headers object
  var headers = new Headers();
  
  // Set the options of the request
  var requestOptions = {
      method: 'GET',
      headers: headers 
  };
  
  // Fetch content from API 
  const response = await fetch(config.api_url, requestOptions);
  const parsed_data = await response.json();
  dataLoading(parsed_data);

}
