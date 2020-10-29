define([

], function (

) {

function FuelBarView(domain) {
    var container = null;

    var subscriptions = null;

    var min = domain["value.min"] || 0;
    if (typeof (min) === "string") {
        min = parseFloat(min);
    }

    var max = domain["value.max"] || 1;
    if (typeof (max) === "string") {
        max = parseFloat(max);
    }

    function getPercentOf(value) {
        return (value - min) / (max - min);
    }

    function createRow(name) {
        var row = document.createElement("tr");
        row.style.width = "100%";
        var head = document.createElement("td");
        head.innerText = name;
        row.appendChild(head);

        var box = document.createElement("td");
        row.appendChild(box);

        var bar = createFuelBar();
        box.appendChild(bar.content);

        return {
            content: row,
            progress: bar,
            setLabel: function (text) {
                head.innerText = text;
            }
        };
    }

    function createFuelBar() {
        var bar = document.createElement("div");
        bar.style.width = "100%";
        bar.style.height = "30px";
        bar.classList.add("plot-display-area");

        var percentage = document.createElement("div");
        bar.appendChild(percentage);
        percentage.classList.add("c-button");
        percentage.classList.add("c-button--major");
        percentage.style.width = "0%";
        percentage.style.height = "100%";
        percentage.style.padding = "0";

        return {
            content: bar,
            setPercent(float) {
                var width = float;
                if (width > 100) {
                    width = 0;
                } else if (width < 0) {
                    width = 100;
                }

                percentage.style.width = width + "%";
                percentage.innerText = Math.round(float) + "%";
            },
            setNormalizedPercent(float) {
                if (!float) {
                    float = 100;
                }

                this.setPercent(100 - float * 100);
            }
        };
    }

    this.show = function (dom) {
        container = document.createElement("table");
        container.style.width = "100%";

        var composition = domain.composition || [];
        subscriptions = [];
        subscriptions.length = composition.length;
        dom.appendChild(container);

        composition.forEach((id, index) => {
            var row = createRow(id.key);
            container.appendChild(row.content);

            openmct.objects.get(id).then(function (cDomain) {
                var allTelemetry = [];
                if (cDomain.telemetry && cDomain.telemetry.values) {
                    allTelemetry = cDomain.telemetry.values.filter((value) => value.format === "float");
                }

                var first = (allTelemetry.length > 0) ? allTelemetry[0] : null;

                subscriptions[index] = openmct.telemetry.subscribe(cDomain, function (data) {
                    if (!first) {
                        return;
                    }

                    row.progress.setNormalizedPercent(getPercentOf(data[first.source || first.key]));
                });
            });
        });
    };

    this.destroy = function () {
    // Remove dom
        if (container) {
            container.remove();
        }

        // Unsubscribe
        if (subscriptions != null) {
            for (var i = 0; i < subscriptions.length; i++) {
                if (subscriptions[i]) {
                    subscriptions[i]();
                }
            }

            subscriptions = null;
        }
    };

    return this;
}

return FuelBarView

});