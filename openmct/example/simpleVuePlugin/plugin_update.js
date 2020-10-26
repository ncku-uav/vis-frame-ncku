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
                var gaugetemplate = require('./Gauge.vue'); //or HelloWorld or ToDoApp
                
                
                // Vue.use(VueGoogleMaps, {
                //     load: {
                //         key: 'AIzaSyAjxxQnLT-aN7qBed0GS9a82rupwYJg-5g',
                //         libraries: 'places',
                //     },
                //     installComponents: true

                // })
                
                return {
                    show: function (container) {
                        vm = new Vue(gaugetemplate.default);
                        //vm.message = 'Nooooo';
                        vm.coordinates = [11.286146,48.082427];
                        vm.rotatePlane = "0";
                        
                        
                        
                        
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
