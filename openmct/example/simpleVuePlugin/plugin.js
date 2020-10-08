//import Vue from 'vue';
//import HelloWorld from './HelloWorld.vue';

define(['vue', './HelloWorld.vue'],//, './Gauge.vue'], //'gauge',

function (Vue, HelloWorld){ //,  GaugeComponent) { //Gauge,


function SimpleVuePlugin() {
    return function install(openmct) {
    
        openmct.types.addType('hello-world', {
            name: 'Hello World',
            description: 'An introduction object',
            creatable: true
        });
        openmct.objectViews.addProvider({
            name: "demo-provider",
            key: "hello-world",
            cssClass: "icon-packet",
            canView: function (d) {
                return d.type === 'hello-world';
            },
            view: function (domainObject) {
                var vm;
                var gaugetemplate = require('./HelloWorld.vue').default; //or HelloWorld or ToDoApp
                //var VueSpeedometer = require('vue-speedometer');
                //Vue.component(VueSpeedometer)

                return {
                    show: function (container) {
                        //vm = new Vue(HelloWorld.default);
                        vm = new Vue(gaugetemplate)
                        
                        
                        container.appendChild(vm.$mount().$el);
                    },
                    destroy: function (container) {
                        vm.$destroy();
                    }
                };
            }
        });

    };
    
}
return SimpleVuePlugin;


});
