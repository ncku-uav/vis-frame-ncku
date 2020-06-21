function getDictionary2() {
    return http.get('/dictionary2.json')
        .then(function (result) {
            return result.data;
        });
}

var objectProvider2 = {
    get: function (identifier) {
        return getDictionary2().then(function (dictionary2) {
            if (identifier.key === 'TFLEX') {
                return {
                    identifier: identifier,
                    name: dictionary2.name,
                    type: 'folder',
                    location: 'ROOT'
                };
            } else {
                var measurement = dictionary2.measurements.filter(function (m) {
                    return m.key === identifier.key;
                })[0];
                return {
                    identifier: identifier,
                    name: measurement.name,
                    type: 'example.telemetry',
                    telemetry: {
                        values: measurement.values
                    },
                    location: 'example.taxonomy2:TFLEX'
                };
            }
        });
    }
};

var compositionProvider2 = {
    appliesTo: function (domainObject) {
        return domainObject.identifier.namespace === 'example.taxonomy2' &&
               domainObject.type === 'folder';
    },
    load: function (domainObject) {
        return getDictionary2()
            .then(function (dictionary2) {
                return dictionary2.measurements.map(function (m) {
                    return {
                        namespace: 'example.taxonomy2',
                        key: m.key
                    };
                });
            });
    }
};

function FLEXOPPlugin() {
    return function install(openmct) {
        openmct.objects.addRoot({
            namespace: 'example.taxonomy2',
            key: 'TFLEX'
        });

        openmct.objects.addProvider('example.taxonomy2', objectProvider2);
        
        openmct.composition.addProvider(compositionProvider2);
    
        openmct.types.addType('example.telemetry', {
            name: 'Example Telemetry Point',
            description: 'Example telemetry point from our happy tutorial.',
            cssClass: 'icon-telemetry'
        });
    }
};
