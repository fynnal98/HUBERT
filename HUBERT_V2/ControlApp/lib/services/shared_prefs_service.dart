import 'package:shared_preferences/shared_preferences.dart';
import 'dart:convert';


class SharedPrefsService {
  late SharedPreferences _prefs;

  Future<void> init() async {
    _prefs = await SharedPreferences.getInstance();
  }

  Future<List<String>> getProfiles() async {
    return _prefs.getStringList('profiles') ?? ['Standard'];
  }

  Future<void> saveProfilesList(List<String> profiles) async {
    await _prefs.setStringList('profiles', profiles);
  }

  Future<String> getSelectedProfile() async {
    return _prefs.getString('selectedProfile') ?? 'Standard';
  }

  Future<void> saveSelectedProfile(String profile) async {
    await _prefs.setString('selectedProfile', profile);
  }

  Future<void> saveProfileParameters(String profile, Map<String, String> parameters) async {
    String jsonData = parameters.isNotEmpty ? parameters.toString() : "{}";
    await _prefs.setString('profile_$profile', jsonData);
  }

  Future<Map<String, dynamic>> loadProfileParameters(String profile) async {
    String? jsonString = _prefs.getString('profile_$profile');
    if (jsonString == null) return {};
    return Map<String, dynamic>.from(jsonString.isNotEmpty ? _convertJsonStringToMap(jsonString) : {});
  }

  Map<String, dynamic> _convertJsonStringToMap(String jsonString) {
    return jsonString.contains('{') ? jsonDecode(jsonString.replaceAll(RegExp(r"\(|\)"), '')) : {};
  }

  /// Speichert das Icon für ein Profil
  Future<void> saveProfileIcon(String profile, String iconLabel) async {
    await _prefs.setString('profile_icon_$profile', iconLabel);
  }

  /// Lädt das gespeicherte Icon für ein Profil
  Future<String?> getProfileIcon(String profile) async {
    return _prefs.getString('profile_icon_$profile');
  }
}
