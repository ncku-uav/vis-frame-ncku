
const fs = require('fs');

function History() {

	// file path of saved history
	fs.readFile('saved_logs/log_dummy.json', 'utf-8', (err, data) => {
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
