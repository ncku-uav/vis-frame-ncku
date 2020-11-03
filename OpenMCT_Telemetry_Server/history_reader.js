
const fs = require('fs');

function History() {

	// file path of saved history
	fs.readFile('saved_logs/DG800_2020-11-03 23-28-12.json', 'utf-8', (err, data) => {
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
