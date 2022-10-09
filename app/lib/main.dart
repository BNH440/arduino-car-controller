import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final FlutterBluePlus flutterBlue = FlutterBluePlus.instance;
  final List<BluetoothDevice> devicesList = <BluetoothDevice>[];
  final Map<Guid, List<int>> readValues = <Guid, List<int>>{};

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  List<BluetoothService> _services = <BluetoothService>[];
  BluetoothDevice? _connectedDevice;

  ListView _buildListViewOfDevices() {
    List<Container> containers = <Container>[];
    for (BluetoothDevice device in widget.devicesList) {
      containers.add(
        Container(
          height: 50,
          child: Row(
            children: <Widget>[
              Expanded(
                child: Column(
                  children: <Widget>[
                    Text(device.name == '' ? '(unknown device)' : device.name),
                    Text(device.id.toString()),
                  ],
                ),
              ),
              MaterialButton(
                color: Colors.blue,
                child: const Text(
                  'Connect',
                  style: TextStyle(color: Colors.white),
                ),
                onPressed: () async {
                  log("connecting...");
                  widget.flutterBlue.stopScan();
                  try {
                    await device.connect();
                  } catch (e) {
                    if (e != 'already_connected') {
                      await device.disconnect();
                      await device.connect();
                    }
                  } finally {
                    _services = await device.discoverServices();
                  }
                  setState(() {
                    _connectedDevice = device;
                  });
                },
              ),
            ],
          ),
        ),
      );
    }

    return ListView(
      padding: const EdgeInsets.all(8),
      children: <Widget>[
        ...containers,
      ],
    );
  }

  ListView _buildConnectDeviceView() {
    List<Widget> containers = <Widget>[
      ListView(
        shrinkWrap: true,
        children: [
          MaterialButton(
              color: Colors.blue,
              child: const Text(
                'Horn',
                style: TextStyle(color: Colors.white),
              ),
              onPressed: () {
                _services[2].characteristics[2].write([0x00]);
              }),
          MaterialButton(
              color: Colors.blue,
              child: const Text(
                'Light On',
                style: TextStyle(color: Colors.white),
              ),
              onPressed: () {
                _services[2].characteristics[0].write([0x01]);
              }),
          MaterialButton(
              color: Colors.blue,
              child: const Text(
                'Light Off',
                style: TextStyle(color: Colors.white),
              ),
              onPressed: () {
                _services[2].characteristics[0].write([0x00]);
              }),
        ],
      ),
      ListView(
        shrinkWrap: true,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              MaterialButton(
                  color: Colors.blue,
                  child: const Text(
                    'Forward',
                    style: TextStyle(color: Colors.white),
                  ),
                  onPressed: () {
                    _services[2].characteristics[3].write([0x01]);
                  }),
            ],
          ),
          Row(mainAxisAlignment: MainAxisAlignment.center, children: [
            MaterialButton(
                color: Colors.blue,
                child: const Text(
                  'Left',
                  style: TextStyle(color: Colors.white),
                ),
                onPressed: () {
                  _services[2].characteristics[3].write([0x03]);
                }),
            MaterialButton(
                color: Colors.blue,
                child: const Text(
                  'Right',
                  style: TextStyle(color: Colors.white),
                ),
                onPressed: () {
                  _services[2].characteristics[3].write([0x04]);
                }),
          ]),
          Row(mainAxisAlignment: MainAxisAlignment.center, children: [
            MaterialButton(
                color: Colors.blue,
                child: const Text(
                  'Backward',
                  style: TextStyle(color: Colors.white),
                ),
                onPressed: () {
                  _services[2].characteristics[3].write([0x02]);
                }),
          ]),
          MaterialButton(
              color: Colors.blue,
              child: const Text(
                'Stop',
                style: TextStyle(color: Colors.white),
              ),
              onPressed: () {
                _services[2].characteristics[3].write([0x00]);
              }),
          MaterialButton(
              onPressed: () async {
                var sub = _services[2].characteristics[1].value.listen((value) {
                  setState(() {
                    widget.readValues[_services[2].characteristics[1].uuid] =
                        value;
                  });
                });
                await _services[2].characteristics[1].read();
                sub.cancel();
                log((await (_services[2].characteristics[1].read()))
                    .toString());
              },
              child: Text(
                  "Light Level: ${(widget.readValues[_services[2].characteristics[1].uuid])?[0]}")),
          MaterialButton(
              onPressed: () async {
                var sub = _services[2].characteristics[4].value.listen((value) {
                  setState(() {
                    widget.readValues[_services[2].characteristics[4].uuid] =
                        value;
                  });
                });
                await _services[2].characteristics[4].read();
                sub.cancel();
                log((await (_services[2].characteristics[4].read()))
                    .toString());
              },
              child: Text(
                  "Temp(F): ${(widget.readValues[_services[2].characteristics[4].uuid])?[0]}")),
        ],
      )
    ];

    return ListView(
      padding: const EdgeInsets.all(8),
      children: <Widget>[
        ...containers,
      ],
    );
  }

  _addDeviceTolist(final BluetoothDevice device) {
    if (!widget.devicesList.contains(device)) {
      setState(() {
        widget.devicesList.add(device);
      });
    }
  }

  @override
  void initState() {
    super.initState();
    widget.flutterBlue.connectedDevices
        .asStream()
        .listen((List<BluetoothDevice> devices) {
      for (BluetoothDevice device in devices) {
        _addDeviceTolist(device);
      }
    });
    widget.flutterBlue.scanResults.listen((List<ScanResult> results) {
      for (ScanResult result in results) {
        _addDeviceTolist(result.device);
      }
    });
    widget.flutterBlue.startScan();
  }

  ListView _buildView() {
    if (_connectedDevice != null) {
      return _buildConnectDeviceView();
    }
    return _buildListViewOfDevices();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: _buildView(),
    );
  }
}
