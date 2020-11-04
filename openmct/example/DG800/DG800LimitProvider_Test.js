/*****************************************************************************
 * Open MCT, Copyright (c) 2014-2020, United States Government
 * as represented by the Administrator of the National Aeronautics and Space
 * Administration. All rights reserved.
 *
 * Open MCT is licensed under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Open MCT includes source code licensed under additional open source
 * licenses. See the Open Source Licenses file (LICENSES.md) included with
 * this source code distribution or the Licensing information page available
 * at runtime from the About dialog for additional information.
 *****************************************************************************/

define([

], function (

) {

    var RED = {
        key: 900,
            cos: 0.9
        },
        YELLOW = {
            key: 500,
            cos: 0.5
        },
        LIMITS = {
            rh: {
                cssClass: "is-limit--upr is-limit--red",
                low: 900,
                high: Number.POSITIVE_INFINITY,
                name: "Red High"
            },
            rl: {
                cssClass: "is-limit--lwr is-limit--red",
                high: -900,
                low: Number.NEGATIVE_INFINITY,
                name: "Red Low"
            },
            yh: {
                cssClass: "is-limit--upr is-limit--yellow",
                low: YELLOW,
                high: RED,
                name: "Yellow High"
            },
            yl: {
                cssClass: "is-limit--lwr is-limit--yellow",
                low: -RED,
                high: -YELLOW,
                name: "Yellow Low"
            }
        };

    function DG800LimitProvider() {

    }

    DG800LimitProvider.prototype.supportsLimits = function (domainObject) {
        return domainObject.type === 'DG800.telemetry';
    };

    DG800LimitProvider.prototype.getLimitEvaluator = function (domainObject) {
        return {
            evaluate: function (datum, valueMetadata) {
                var range = valueMetadata && valueMetadata.key;
                var count = 0;
                if(count < 1){
                console.log('range: '+ YELLOW[range])
                count = count+1;
                }

                if (datum[range] > RED[range]) {
                    return LIMITS.rh;
                }

                if (datum[range] < -RED[range]) {
                    return LIMITS.rl;
                }

                if (datum[range] > YELLOW[range]) {
                    return LIMITS.yh;
                }

                if (datum[range] < -YELLOW[range]) {
                    return LIMITS.yl;
                }
            }
        };
    };

    return DG800LimitProvider;
});
