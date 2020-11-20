
define([
    //"./Aircraft_44LimitProvider_Test",
], function (
    //Aircraft_44LimitProvider
) {

    function Aircraft_44Plugin() {

        function getAircraft_44Dictionary() {
            return fetch('/example/Aircraft_44/Aircraft_44dictionary.json').then(function (response) {
                return response.json();
            });

        }

        // An object provider builds Domain Objects
        var Aircraft_44_objectProvider = {
            get: function (identifier) {
                return getAircraft_44Dictionary().then(function (dictionary) {
                    //console.log("Aircraft_44-dictionary-plugin.js: identifier.key = " + identifier.key);
                    if (identifier.key === 'Aircraft_44') {
                        return {
                            identifier: identifier,
                            name: dictionary.name,
                            type: 'folder',
                            location: 'ROOT'
                        };
                    } else {
                        var measurement = dictionary.measurements.filter(function (m) {
                            return m.key === identifier.key;
                        })[0];

                        return {
                            identifier: identifier,
                            name: measurement.name,
                            type: 'Aircraft_44.telemetry',
                            telemetry: {
                                values: measurement.values
                            },
                            location: 'Aircraft_44.taxonomy:Aircraft_44'
                        };
                    }
                });
            }
        };

        // The composition of a domain object is the list of objects it contains, as shown (for example) in the tree for browsing.
        // Can be used to populate a hierarchy under a custom root-level object based on the contents of a telemetry dictionary.
        // "appliesTo"  returns a boolean value indicating whether this composition provider applies to the given object
        // "load" returns an array of Identifier objects (like the channels this telemetry stream offers)
        var Aircraft_44_compositionProvider = {
            appliesTo: function (domainObject) {
                return domainObject.identifier.namespace === 'Aircraft_44.taxonomy'
                    && domainObject.type === 'folder';
            },
            load: function (domainObject) {
                return getAircraft_44Dictionary()
                    .then(function (dictionary) {
                        return dictionary.measurements.map(function (m) {
                            return {
                                namespace: 'Aircraft_44.taxonomy',
                                key: m.key
                            };
                        });
                    });
            }
        };

        return function install(openmct) {
            // The addRoot function takes an "object identifier" as an argument
            openmct.objects.addRoot({
                namespace: 'Aircraft_44.taxonomy',
                key: 'Aircraft_44'
            });

            openmct.objects.addProvider('Aircraft_44.taxonomy', Aircraft_44_objectProvider);

            openmct.composition.addProvider(Aircraft_44_compositionProvider);

            //openmct.telemetry.addProvider(new Aircraft_44LimitProvider());

            openmct.types.addType('Aircraft_44.telemetry', {
                name: 'Aircraft_44 Telemetry Point',
                description: 'Telemetry of Aircraft_44',
                cssClass: 'icon-telemetry'
            });
        };
    }

    return Aircraft_44Plugin;
});
