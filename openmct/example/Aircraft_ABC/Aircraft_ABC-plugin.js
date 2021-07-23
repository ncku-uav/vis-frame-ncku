
define([
    //"./Aircraft_ABCLimitProvider_Test",
], function (
    //Aircraft_ABCLimitProvider
) {

    function Aircraft_ABCPlugin() {

        function getAircraft_ABCDictionary() {
            return fetch('/example/Aircraft_ABC/Aircraft_ABCdictionary.json').then(function (response) {
                return response.json();
            });

        }

        // An object provider builds Domain Objects
        var Aircraft_ABC_objectProvider = {
            get: function (identifier) {
                return getAircraft_ABCDictionary().then(function (dictionary) {
                    //console.log("Aircraft_ABC-dictionary-plugin.js: identifier.key = " + identifier.key);
                    if (identifier.key === 'Aircraft_ABC') {
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
                            type: 'Aircraft_ABC.telemetry',
                            telemetry: {
                                values: measurement.values
                            },
                            location: 'Aircraft_ABC.taxonomy:Aircraft_ABC'
                        };
                    }
                });
            }
        };

        // The composition of a domain object is the list of objects it contains, as shown (for example) in the tree for browsing.
        // Can be used to populate a hierarchy under a custom root-level object based on the contents of a telemetry dictionary.
        // "appliesTo"  returns a boolean value indicating whether this composition provider applies to the given object
        // "load" returns an array of Identifier objects (like the channels this telemetry stream offers)
        var Aircraft_ABC_compositionProvider = {
            appliesTo: function (domainObject) {
                return domainObject.identifier.namespace === 'Aircraft_ABC.taxonomy'
                    && domainObject.type === 'folder';
            },
            load: function (domainObject) {
                return getAircraft_ABCDictionary()
                    .then(function (dictionary) {
                        return dictionary.measurements.map(function (m) {
                            return {
                                namespace: 'Aircraft_ABC.taxonomy',
                                key: m.key
                            };
                        });
                    });
            }
        };

        return function install(openmct) {
            // The addRoot function takes an "object identifier" as an argument
            openmct.objects.addRoot({
                namespace: 'Aircraft_ABC.taxonomy',
                key: 'Aircraft_ABC'
            });

            openmct.objects.addProvider('Aircraft_ABC.taxonomy', Aircraft_ABC_objectProvider);

            openmct.composition.addProvider(Aircraft_ABC_compositionProvider);

            //openmct.telemetry.addProvider(new Aircraft_ABCLimitProvider());

            openmct.types.addType('Aircraft_ABC.telemetry', {
                name: 'Aircraft_ABC Telemetry Point',
                description: 'Telemetry of Aircraft_ABC',
                cssClass: 'icon-telemetry'
            });
        };
    }

    return Aircraft_ABCPlugin;
});
