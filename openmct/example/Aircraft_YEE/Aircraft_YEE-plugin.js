
define([
    //"./Aircraft_YEELimitProvider_Test",
], function (
    //Aircraft_YEELimitProvider
) {

    function Aircraft_YEEPlugin() {

        function getAircraft_YEEDictionary() {
            return fetch('/example/Aircraft_YEE/Aircraft_YEEdictionary.json').then(function (response) {
                return response.json();
            });

        }

        // An object provider builds Domain Objects
        var Aircraft_YEE_objectProvider = {
            get: function (identifier) {
                return getAircraft_YEEDictionary().then(function (dictionary) {
                    //console.log("Aircraft_YEE-dictionary-plugin.js: identifier.key = " + identifier.key);
                    if (identifier.key === 'Aircraft_YEE') {
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
                            type: 'Aircraft_YEE.telemetry',
                            telemetry: {
                                values: measurement.values
                            },
                            location: 'Aircraft_YEE.taxonomy:Aircraft_YEE'
                        };
                    }
                });
            }
        };

        // The composition of a domain object is the list of objects it contains, as shown (for example) in the tree for browsing.
        // Can be used to populate a hierarchy under a custom root-level object based on the contents of a telemetry dictionary.
        // "appliesTo"  returns a boolean value indicating whether this composition provider applies to the given object
        // "load" returns an array of Identifier objects (like the channels this telemetry stream offers)
        var Aircraft_YEE_compositionProvider = {
            appliesTo: function (domainObject) {
                return domainObject.identifier.namespace === 'Aircraft_YEE.taxonomy'
                    && domainObject.type === 'folder';
            },
            load: function (domainObject) {
                return getAircraft_YEEDictionary()
                    .then(function (dictionary) {
                        return dictionary.measurements.map(function (m) {
                            return {
                                namespace: 'Aircraft_YEE.taxonomy',
                                key: m.key
                            };
                        });
                    });
            }
        };

        return function install(openmct) {
            // The addRoot function takes an "object identifier" as an argument
            openmct.objects.addRoot({
                namespace: 'Aircraft_YEE.taxonomy',
                key: 'Aircraft_YEE'
            });

            openmct.objects.addProvider('Aircraft_YEE.taxonomy', Aircraft_YEE_objectProvider);

            openmct.composition.addProvider(Aircraft_YEE_compositionProvider);

            //openmct.telemetry.addProvider(new Aircraft_YEELimitProvider());

            openmct.types.addType('Aircraft_YEE.telemetry', {
                name: 'Aircraft_YEE Telemetry Point',
                description: 'Telemetry of Aircraft_YEE',
                cssClass: 'icon-telemetry'
            });
        };
    }

    return Aircraft_YEEPlugin;
});
