import 'package:flutter/material.dart';
//import 'package:web_socket_channel/web_socket_channel.dart';
import 'package:web_socket_channel/io.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Phoi Do ',
      theme: ThemeData(
        colorScheme:
            ColorScheme.fromSeed(seedColor: Color.fromARGB(255, 20, 152, 228)),
        useMaterial3: true,
      ),
      debugShowCheckedModeBanner: false,
      home: const MyHomePage(title: 'Phoi do smart'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;
  late bool ledstatus; //boolean value to track LED status, if its ON or OFF
  late IOWebSocketChannel channel;
  late bool connected; //boolean value to track if WebSocket is connected

  @override
  void initState() {
    ledstatus = false; //initially leadstatus is off so its FALSE
    connected = false; //initially connection status is "NO" so its FALSE

    Future.delayed(Duration.zero, () async {
      channelconnect(); //connect to WebSocket wth NodeMCU
    });

    super.initState();
  }

  void _incrementCounter() {
    //sendcmd("poweroff");

    setState(() {
      _counter++;
    });
  }

  void _run() {
    sendcmd("run");
  }

  void _backward() {
    sendcmd("backward");
  }

  void _forward() {
    sendcmd("forward");
  }

  void _stop() {
    sendcmd("stop");
  }

  channelconnect() {
    //function to connect
    try {
      channel = IOWebSocketChannel.connect(
          "ws://192.168.4.80:80"); //channel IP : Port
      channel.stream.listen(
        (message) {
          print(message);
          setState(() {
            if (message == "connected") {
              connected = true; //message is "connected" from NodeMCU
            } else if (message == "poweron:success") {
              ledstatus = true;
            } else if (message == "poweroff:success") {
              ledstatus = false;
            }
          });
        },
        onDone: () {
          //if WebSocket is disconnected
          print("Web socket is closed");
          setState(() {
            connected = false;
          });
        },
        onError: (error) {
          print(error.toString());
        },
      );
    } catch (_) {
      print("error on connecting to websocket.");
    }
  }

  Future<void> sendcmd(String cmd) async {
    if (connected == true) {
      if (ledstatus == false &&
          cmd != "poweron" &&
          cmd != "poweroff" &&
          cmd != "stop" &&
          cmd != "backward" &&
          cmd != "forward") {
        print("Send the valid command");
      } else {
        channel.sink.add(cmd); //sending Command to NodeMCU
      }
    } else {
      channelconnect();
      print("Websocket is not connected.");
    }
  }

  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          //mainAxisAlignment: MainAxisAlignment.center,
          //crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[
            const Text(
              'You have pushed the button this many times:',
            ),
            Text(
              '$_counter',
              style: Theme.of(context).textTheme.headlineMedium,
            ),
            const SizedBox(
              height: 100,
            ),
            ElevatedButton(
              onPressed: _backward,
              style: ElevatedButton.styleFrom(
                  fixedSize: Size(120, 34), // specify width, height
                  shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(
                    20,
                  ))),
              child: Text("Backward"),
            ),
            const SizedBox(
              height: 30,
            ),
            ElevatedButton(onPressed: _stop, child: Text("Stop")),
            const SizedBox(
              height: 30,
            ),
            ElevatedButton(onPressed: _forward, child: Text("Forward"))
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
