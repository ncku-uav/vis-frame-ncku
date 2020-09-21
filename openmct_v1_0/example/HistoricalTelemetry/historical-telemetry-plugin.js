/**
 * Basic historical telemetry plugin.
 */

define([

], function (

) {

    function HistoricalTelemetryPlugin(desired_domain_object_type, serverURL) {
    //function HistoricalTelemetryPlugin() {
        

        return function install(openmct) {
            //var desired_domain_object_type = 'TFLEX.telemetry';
            var port = 8090;
            var provider = {
                supportsRequest: function (domainObject) {
                    return domainObject.type === desired_domain_object_type;
                },
                request: function (domainObject, options) {
                    var url = 'http://10.183.35.90:' + port + serverURL +
                        domainObject.identifier.key +
                        '?start=' + options.start +
                        '&end=' + options.end;
                    console.log('historical-telemetry-plugin.js: send request = ' + url);
                    //http gibts nicht mehr!!!!!!!!!!!!!!!!!
                    return fetch(url).then(function (resp){
                        console.log(resp)
                        return resp.json();
                    });
                    // return http.get(url)
                    //     .then(function (resp) {
                    //         return resp.data;
                    //     });
                }
            };

            openmct.telemetry.addProvider(provider);
        }
    };

    return HistoricalTelemetryPlugin;
});