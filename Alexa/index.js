'use strict';
var Alexa = require("alexa-sdk");
// var $ = require("jquery");
var request = require("request");
var APP_ID = '';
var spacestatus ;
var dummy1;
var dummy2;
var dummy3;
var arr;
var lastKnown;
var languageStrings = {
    "en": {
        "translation": {
            "SKILL_NAME": "PolyPark",
            "HELP_MESSAGE": "Ask me how many spaces are available or ask me what is the status of space 1",
            "HELP_REPROMPT": "gfd",
            "STOP_MESSAGE": "Goodbye!"
        }
    }
};

exports.handler = function(event, context, callback) {
    var alexa = Alexa.handler(event, context);
    alexa.APP_ID = APP_ID;
    alexa.resources = languageStrings;
    alexa.registerHandlers(handlers);
    alexa.execute();
};
var handlers = {
    'LaunchRequest': function() {
		const self = this;
        getstatus(function(){self.emit(':ask', "Welcome to polypark ..... To use the app ask me how many spaces are available or what is the space status ... or are there any free spaces left ");});
		//this.emit(':ask', "Welcome to polypark ..... To use the app ask me how many spaces are avalable or what is the space status");
    },
	'Space': function(){
		const self = this;
		getstatus(
		function(){
		console.log(spacestatus);
		console.log(lastKnown);
		if (spacestatus == null || spacestatus === "undefined" || spacestatus == null){
			// if the last known status is not undefined, connection has been made
			if (lastKnown == null || lastKnown === "undefined" || lastKnown == null){
				// if both are undefined, connection to the server was never made.
			self.emit(':tell',"Cannot connect to the server, please try again ");}
				else{ self.emit(':ask',"Cannot connect to server, last known status of space was " + lastKnown);
		}}else{ self.emit(':ask',"space 1 is " + spacestatus);
		}
		});
	},

	'math': function(){
	const self = this;

		getstatus(
		function(){
		if (arrCheck()){
	self.emit(':ask', "Error in network connection to server. Please try again");
		}else{
self.emit(':ask', "There are currently "+ domath() + " out of 4 spaces open");
	}});
	
	},
	
	'parkingaval': function() {
        if(spacestatus == "OPEN"){
		this.emit(':ask', "Yes, there is currently a spot open better hurry before its gone!")
		}else{this.emit(':ask', "sorry, you are too late there are no spaces left")}
    },
	'AMAZON.HelpIntent': function() {
        this.emit(':ask', speechOutput, reprompt);
    },
    
	'AMAZON.CancelIntent': function() {
        this.emit(':tell', this.t("STOP_MESSAGE"));
    },
    
	'AMAZON.StopIntent': function() {
        this.emit(':tell', this.t("STOP_MESSAGE"));
    },
	'VIP': function() {
        this.emit(':ask', "Thank you for purchasing a VIP spot ................. your data has been recorded ............... Please drive into any spot marked VIP");
    },
	  'Unhandled': function() {
		this.emit(':tell', "Sorry, there has been an unknown error.");

		  
	  }
	
};
// connects to server and updates the space status
 function getstatus(_callback){
request('xxxxxxx', function (error, response, body) {
  console.log('error:', error); // Print the error if one occurred
  console.log('statusCode:', response && response.statusCode); // Print the response status code if a response was received
  console.log('body:', body); // Print the HTML for the polypark homepage.
  console.log(JSON.parse(body));  
  console.log("Get status called");
	// assign the variables to the data
	if(error){lastKnown = spacestatus;	// error handling, if the status is not undefined, we can assume data came through and set it to last known good varible.
	return;
	}
	spacestatus = JSON.parse(body).Space1;  
	dummy1 = JSON.parse(body).Dummy1;
	dummy2 = JSON.parse(body).Dummy2;
	dummy3 = JSON.parse(body).Dummy3;
	arr = [spacestatus,dummy1,dummy2,dummy3];
	
	 _callback();
	});
}

function domath(){
var spaceCount = 4;
for (var i=0; i<arr.length; i++){
	if(arr[i] == "CLOSED"){
		spaceCount--;
		}
	}
	return spaceCount;
}

function arrCheck(){
	if (arr.length == 0 ){return true}
	else{return false;}
}
function calltest(){
	return this.emit(':ask', "Welcome to polypark ..... To use the app ask me how many spaces are avalable or what is the space status"); 
	
}
	
