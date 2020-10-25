
const fs = require('fs');

function History() {


	fs.readFile('saved_logs/DG800_2020-10-23 1:9:49.json', 'utf-8', (err, data) => {
		if (err) {
			throw err;
		}

		// parse JSON object
		this.history = JSON.parse(data.toString());
	});


	console.log("History initialized!");

};



module.exports = function () {
	return new History()
};
