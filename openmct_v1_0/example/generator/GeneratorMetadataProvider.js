define([
    'lodash'
], function (
    _
) {

    var METADATA_BY_TYPE = {
        'generator': {
            values: [
                {
                    key: "name",
                    name: "Name",
                    format: "string"
                },
                {
                    key: "utc",
                    name: "Time",
                    format: "utc",
                    hints: {
                        domain: 1
                    }
                },
                {
                    key: "yesterday",
                    name: "Yesterday",
                    format: "utc",
                    hints: {
                        domain: 2
                    }
                },
                // Need to enable "LocalTimeSystem" plugin to make use of this
                // {
                //     key: "local",
                //     name: "Time",
                //     format: "local-format",
                //     source: "utc",
                //     hints: {
                //         domain: 3
                //     }
                // },
                {
                    key: "sin",
                    name: "Sine",
                    formatString: '%0.2f',
                    hints: {
                        range: 1
                    }
                },
                {
                    key: "cos",
                    name: "Cosine",
                    formatString: '%0.2f',
                    hints: {
                        range: 2
                    }
                }
            ]
        },
        'example.state-generator': {
            values: [
                {
                    key: "name",
                    name: "Name",
                    format: "string"
                },
                {
                    key: "utc",
                    name: "Time",
                    format: "utc",
                    hints: {
                        domain: 1
                    }
                },
                {
                    key: "local",
                    name: "Time",
                    format: "utc",
                    source: "utc",
                    hints: {
                        domain: 2
                    }
                },
                {
                    key: "state",
                    source: "value",
                    name: "State",
                    format: "enum",
                    enumerations: [
                        {
                            value: 0,
                            string: "OFF"
                        },
                        {
                            value: 1,
                            string: "ON"
                        }
                    ],
                    hints: {
                        range: 1
                    }
                },
                {
                    key: "value",
                    name: "Value",
                    hints: {
                        range: 2
                    }
                }
            ]
        }
    }

    function GeneratorMetadataProvider() {

    }

    GeneratorMetadataProvider.prototype.supportsMetadata = function (domainObject) {
        return METADATA_BY_TYPE.hasOwnProperty(domainObject.type);
    };

    GeneratorMetadataProvider.prototype.getMetadata = function (domainObject) {
        return Object.assign(
            {},
            domainObject.telemetry,
            METADATA_BY_TYPE[domainObject.type]
        );
    };

    return GeneratorMetadataProvider;

});