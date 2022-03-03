#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

time_t getNow();
int getHours(time_t *);
int getMinutes(time_t *);
int getSeconds(time_t *);

void renderCurrentTime(struct config *, time_t *, string []);

struct config {
  // total LEDs
  int total = 165;

  // main clock
  int totalHours = 13;
  int hoursStart = 6;
  int intervalHours = 12;
  int intervalHalves = intervalHours / 2;
  int intervalQuarters = intervalHours / 4;
  int clockStart = 0;
  int clockEnd = totalHours * intervalHours;

  // minutes counter
  int intervalMinutes = 5;
  int minutesStart = clockEnd;
  int minutesEnd = minutesStart + intervalMinutes;

  // seconds counter
  int intervalSeconds = 4;
  int secondsRepresentation = 15;
  int secondsStart = minutesEnd;
  int secondsEnd = secondsStart + intervalSeconds;

  // colors
  string colorTicksHours     = "\033[0;33mx\033[0m"; // black
  string colorTicksHalves    = "\033[0;34mx\033[0m"; // blue
  string colorTicksQuarters  = "\033[0;31mx\033[0m"; // red
  string colorFuture         = "\033[0;37mx\033[0m"; // white
  string colorPast           = "\033[0;32mx\033[0m"; // green
  string colorCounterMinutes = "\033[0;34mx\033[0m"; // blue
  string colorCounterSeconds = "\033[0;32mx\033[0m"; // green


};

int main() {
  config cfg;
  string colors[cfg.total];

  // log all the parameters
  cout << "                total: " << cfg.total                 << endl;
  cout << "           totalHours: " << cfg.totalHours            << endl;
  cout << "           hoursStart: " << cfg.hoursStart            << endl;
  cout << "        intervalHours: " << cfg.intervalHours         << endl;
  cout << "       intervalHalves: " << cfg.intervalHalves        << endl;
  cout << "     intervalQuarters: " << cfg.intervalQuarters      << endl;
  cout << "           clockStart: " << cfg.clockStart            << endl;
  cout << "             clockEnd: " << cfg.clockEnd              << endl;
  cout << "      intervalMinutes: " << cfg.intervalMinutes       << endl;
  cout << "         minutesStart: " << cfg.minutesStart          << endl;
  cout << "           minutesEnd: " << cfg.minutesEnd            << endl;
  cout << "      intervalSeconds: " << cfg.intervalSeconds       << endl;
  cout << "secondsRepresentation: " << cfg.secondsRepresentation << endl;
  cout << "         secondsStart: " << cfg.secondsStart          << endl;
  cout << "           secondsEnd: " << cfg.secondsEnd            << endl;
  cout << "      colorTicksHours: " << cfg.colorTicksHours       << endl;
  cout << "     colorTicksHalves: " << cfg.colorTicksHalves      << endl;
  cout << "   colorTicksQuarters: " << cfg.colorTicksQuarters    << endl;
  cout << "          colorFuture: " << cfg.colorFuture           << endl;
  cout << "            colorPast: " << cfg.colorPast             << endl;
  cout << "  colorCounterMinutes: " << cfg.colorCounterMinutes   << endl;
  cout << "  colorCounterSeconds: " << cfg.colorCounterSeconds   << endl;

  // initialize colors
  for (int i = cfg.clockStart; i < cfg.clockEnd; i++) {
    cout << i << "\t";
    if (i % cfg.intervalHours == 0) {
      cout << "hours";
      colors[i] = cfg.colorTicksHours;
    } else if (i % cfg.intervalHalves == 0) {
      cout << "halves";
      colors[i] = cfg.colorTicksHalves;
    } else if (i % cfg.intervalQuarters == 0) {
      cout << "quarters";
      colors[i] = cfg.colorTicksQuarters;
    } else {
      cout << "minutes";
      colors[i] = cfg.colorFuture;
    }
    cout << endl;
  }

  for (int i = cfg.minutesStart; i < cfg.minutesEnd; i++) {
    cout << i << "\t";
    cout << "minutes counter";
    cout << endl;
    colors[i] = cfg.colorFuture;
  }

  for (int i = cfg.secondsStart; i < cfg.secondsEnd; i++) {
    cout << i << "\t";
    cout << "seconds counter";
    cout << endl;
    colors[i] = cfg.colorFuture;
  }

  time_t now = getNow();
  renderCurrentTime(&cfg, &now, colors);

  return 0;
}

time_t getNow() {
  return chrono::system_clock::to_time_t(chrono::system_clock::now());
}

int getHours(time_t *time) {
  return localtime(time)->tm_hour;
}

int getMinutes(time_t *time) {
  return localtime(time)->tm_min;
}

int getSeconds(time_t *time) {
  return localtime(time)->tm_sec;
}

void renderCurrentTime(config *cfg, time_t *time, string colors[]) {
  cout << "renderCurrentTime: " << ctime(time) << endl;

  int hours   = getHours(time);
  int minutes = getMinutes(time);
  int seconds = getSeconds(time);

  int currentTimeIndex = cfg->clockStart;
  currentTimeIndex += (hours - cfg->hoursStart) * cfg->intervalHours;
  currentTimeIndex += minutes / cfg->intervalMinutes;
  cout << "cfg->clockStart: " << cfg->clockStart << endl;
  cout << "hours: " << hours << endl;
  cout << "cfg->hoursStart: " << cfg->hoursStart << endl;
  cout << "cfg->intervalHours: " << cfg->intervalHours << endl;
  cout << "currentTimeIndex: " << currentTimeIndex << endl;


  int currentMinuteIndex = cfg->minutesStart;
  currentMinuteIndex += minutes / cfg->intervalMinutes;

  int currentSecondIndex = cfg->secondsStart;
  currentSecondIndex += seconds / cfg->secondsRepresentation;

  for (int i = cfg->clockStart; i < cfg->clockEnd; i++) {
    cout << i << "\t";
    if (i < currentTimeIndex) {
      cout << cfg->colorPast;
    } else {
      cout << colors[i];
    }
    cout << endl;
  }

  for (int i = cfg->minutesStart; i < cfg->minutesEnd; i++) {
    cout << i << "\t";
    if (i < currentMinuteIndex) {
      cout << cfg->colorCounterMinutes;
    } else {
      cout << colors[i];
    }
    cout << endl;
  }

  for (int i = cfg->secondsStart; i < cfg->secondsEnd; i++) {
    cout << i << "\t";
    if (i < currentSecondIndex) {
      cout << cfg->colorCounterSeconds;
    } else {
      cout << colors[i];
    }
    cout << endl;
  }
}
