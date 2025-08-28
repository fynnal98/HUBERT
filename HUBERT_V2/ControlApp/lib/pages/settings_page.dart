import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart' show rootBundle;
import '../services/udp_service.dart';
import '../services/shared_prefs_service.dart';
import 'drone_controller_page.dart';

class SettingsPage extends StatefulWidget {
  const SettingsPage({Key? key}) : super(key: key);

  @override
  _SettingsPageState createState() => _SettingsPageState();
}

class _SettingsPageState extends State<SettingsPage> {
  final Map<String, TextEditingController> parameterControllers = {};
  late UDPService udpService;
  late SharedPrefsService sharedPrefsService;
  List<String> profiles = ['Standard'];
  late String selectedProfile;
  late String selectedIcon;
  Map<String, dynamic> parameterJson = {};

  final List<Map<String, dynamic>> availableIcons = [
    {'icon': Icons.person, 'label': 'Person'},
    {'icon': Icons.flight, 'label': 'Helikopter'},
    {'icon': Icons.phone_android, 'label': 'Handy'},
    {'icon': Icons.settings, 'label': 'Einstellungen'},
  ];

  @override
  void initState() {
    super.initState();
    udpService = UDPService();
    sharedPrefsService = SharedPrefsService();
    sharedPrefsService.init().then((_) async {
      await _loadProfiles();
      await _loadIconForProfile(selectedProfile);
    });
    selectedIcon = 'Person';  // Standard-Icon
  }

  Future<void> _loadProfiles() async {
    profiles = await sharedPrefsService.getProfiles();
    selectedProfile = await sharedPrefsService.getSelectedProfile();
    await _loadJsonParameters();
  }

  Future<void> _loadJsonParameters() async {
    try {
      String jsonString = await rootBundle.loadString('assets/database.json');
      setState(() {
        parameterJson = jsonDecode(jsonString);
        _setupControllersFromJson(parameterJson, "");
      });
      await _loadParametersForSelectedProfile();
    } catch (e) {
      print("Fehler beim Laden der JSON-Datei: $e");
    }
  }

  Future<void> _loadIconForProfile(String profile) async {
    String? savedIcon = await sharedPrefsService.getProfileIcon(profile);
    setState(() {
      selectedIcon = savedIcon ?? 'Person';
    });
  }

  Future<void> _loadParametersForSelectedProfile() async {
    Map<String, dynamic> savedData = await sharedPrefsService.loadProfileParameters(selectedProfile);
    if (savedData.isNotEmpty) {
      savedData.forEach((key, value) {
        if (parameterControllers.containsKey(key)) {
          parameterControllers[key]?.text = value.toString();
        }
      });
    }
  }

  void _setupControllersFromJson(Map<String, dynamic> json, String parentPath) {
    json.forEach((key, value) {
      final currentPath = parentPath.isEmpty ? key : "$parentPath/$key";

      if (value is Map<String, dynamic>) {
        _setupControllersFromJson(value, currentPath);
      } else {
        parameterControllers[currentPath] = TextEditingController(text: value.toString());
      }
    });
  }

  Future<void> _changeProfile(String profile) async {
    setState(() {
      selectedProfile = profile;
    });
    await sharedPrefsService.saveSelectedProfile(profile);
    await _loadParametersForSelectedProfile();
    await _loadIconForProfile(profile);
  }

  Future<void> _addNewProfile() async {
    TextEditingController profileController = TextEditingController();
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Neues Profil erstellen'),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            TextField(
              controller: profileController,
              decoration: const InputDecoration(hintText: 'Profilname eingeben'),
            ),
            const SizedBox(height: 10),
            const Text('Icon auswählen:'),
            Wrap(
              spacing: 8,
              children: availableIcons.map((iconData) {
                return ChoiceChip(
                  label: Icon(iconData['icon']),
                  selected: selectedIcon == iconData['label'],
                  onSelected: (bool selected) {
                    setState(() {
                      selectedIcon = iconData['label'];
                    });
                  },
                );
              }).toList(),
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: const Text('Abbrechen'),
          ),
          TextButton(
            onPressed: () async {
              String newProfile = profileController.text.trim();
              if (newProfile.isNotEmpty && !profiles.contains(newProfile)) {
                setState(() {
                  profiles.add(newProfile);
                  selectedProfile = newProfile;
                  sharedPrefsService.saveProfilesList(profiles);
                  sharedPrefsService.saveSelectedProfile(newProfile);
                  sharedPrefsService.saveProfileIcon(newProfile, selectedIcon);
                });
                ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('Neues Profil "$newProfile" hinzugefügt.')));
              }
              Navigator.of(context).pop();
            },
            child: const Text('Hinzufügen'),
          ),
        ],
      ),
    );
  }

  Future<void> _deleteProfile() async {
    if (profiles.length > 1) {
      setState(() {
        profiles.remove(selectedProfile);
        selectedProfile = profiles[0];  // Wähle das erste verbleibende Profil
        sharedPrefsService.saveProfilesList(profiles);
        sharedPrefsService.saveSelectedProfile(selectedProfile);
      });
      ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Profil erfolgreich gelöscht.')));
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Das letzte Profil kann nicht gelöscht werden.')));
    }
  }

  Future<void> _sendLiveParameters() async {
    for (final entry in parameterControllers.entries) {
      await udpService.sendParameter(entry.key, entry.value.text);
    }
    ScaffoldMessenger.of(context).showSnackBar(const SnackBar(content: Text('Parameter live gesendet.')));
  }

  Future<void> _saveProfile() async {
    // Lokales Speichern in der App
    Map<String, String> currentProfileData = {};
    parameterControllers.forEach((key, controller) {
      currentProfileData[key] = controller.text;
    });
    await sharedPrefsService.saveProfileParameters(selectedProfile, currentProfileData);

    // Befehl zum Speichern auf dem ESP32 senden
    await udpService.sendSaveCommand();

    ScaffoldMessenger.of(context).showSnackBar(const SnackBar(
      content: Text('Profil lokal gespeichert und JSON auf dem ESP aktualisiert.'),
    ));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Einstellungen'),
        actions: [
          IconButton(
            icon: const Icon(Icons.sports_esports),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const DroneControllerPage()),
              );
            },
          ),
        ],
      ),
      body: Column(
        children: [
          const SizedBox(height: 20),
          CircleAvatar(
            radius: 50,
            backgroundColor: Colors.grey[200],
            child: Icon(
              availableIcons.firstWhere((element) => element['label'] == selectedIcon)['icon'],
              size: 50,
              color: Colors.blue,
            ),
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              DropdownButton<String>(
                value: selectedProfile,
                items: profiles.map((profile) {
                  return DropdownMenuItem(
                    value: profile,
                    child: Text(profile),
                  );
                }).toList(),
                onChanged: (value) {
                  if (value != null) {
                    _changeProfile(value);
                  }
                },
              ),
              IconButton(
                icon: const Icon(Icons.add),
                onPressed: _addNewProfile,
              ),
              IconButton(
                icon: const Icon(Icons.delete),
                onPressed: _deleteProfile,
              ),
            ],
          ),
          Expanded(
            child: parameterJson.isEmpty
                ? const Center(child: CircularProgressIndicator())
                : Padding(
              padding: const EdgeInsets.all(16.0),
              child: ListView(
                children: _buildExpandableGroups(parameterJson, ""),
              ),
            ),
          ),
        ],
      ),
      floatingActionButton: Row(
        mainAxisAlignment: MainAxisAlignment.end,
        children: [
          FloatingActionButton(
            onPressed: _sendLiveParameters,
            child: const Icon(Icons.send),
          ),
          const SizedBox(width: 16),
          FloatingActionButton(
            onPressed: _saveProfile,
            child: const Icon(Icons.save),
          ),
        ],
      ),
    );
  }

  List<Widget> _buildExpandableGroups(Map<String, dynamic> json, String parentPath) {
    return json.entries.map((entry) {
      final key = entry.key;
      final value = entry.value;
      final currentPath = parentPath.isEmpty ? key : "$parentPath/$key";

      if (value is Map<String, dynamic>) {
        return ExpansionTile(
          title: Text(key, style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
          children: _buildExpandableGroups(value, currentPath),
        );
      } else {
        final controller = parameterControllers[currentPath];
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 8.0),
          child: TextField(
            controller: controller,
            decoration: InputDecoration(
              labelText: currentPath,
              border: const OutlineInputBorder(),
            ),
            keyboardType: TextInputType.text,
          ),
        );
      }
    }).toList();
  }
}
