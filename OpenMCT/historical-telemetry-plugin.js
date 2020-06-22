/**
 * Basic historical telemetry plugin.
 */

function HistoricalTelemetryPlugin(desired_domain_object_type,port) {
    return function install (openmct) {
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
                return http.get(url)
                    .then(function (resp) {
                        return resp.data;
                    });
            }
        };
    
        openmct.telemetry.addProvider(provider);
    }
}
