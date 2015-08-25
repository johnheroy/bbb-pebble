// Set callback for the app ready event
Pebble.addEventListener('ready', function(e) {
  console.log('connect!');
  console.log(e.type);
});

// Set callback for appmessage events
Pebble.addEventListener('appmessage', function(e) {
  console.log('message');
  
  console.log('sending static message');
  Pebble.sendAppMessage({
            'stops': 'stops'});
});