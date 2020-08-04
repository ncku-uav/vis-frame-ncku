/**
 * Basic historical telemetry plugin.
 */

define([

], function (

) {

    function HistoricalTelemetryPlugin(desired_domain_object_type,port) {
    //function HistoricalTelemetryPlugin() {
        

        return function install(openmct) {
            //var desired_domain_object_type = 'TFLEX.telemetry';
            //var port = 8091;
            var provider = {
                supportsRequest: function (domainObject) {
                    return domainObject.type === desired_domain_object_type;
                },
                request: function (domainObject, options) {
                    var url = 'http://localhost:' + port + '/telemetry/' +
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