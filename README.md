# Eurobot2018

Anmerkungen bzgl. der Gestaltung des Roboters von David Buhren:
Ich habe mir ein paar Gedanken gemacht, welche Komponenten der Roboter haben sollte.
- Vorausgesetzt man fährt ausschließlich vorwärts und niemals rückwärts, so ist es okay wenn man auf der Rückseite keine Sensoren anbringt. Es sollte daher reichen, wenn vorne mehrer nach vorn gerichtete Distanzsensoren angebracht sind, die sowohl Gegner, als auch Wände detektieren.
- Es sollte angestrebt werden, für jede Roboteraktion möglichst nur einen Aktor zu verwenden.
- Der Bau von Gebäuden entsprechend der Konstruktionsvorschriften ist sehr aufwendig (Bildverarbeitung + Sortierung + Stapeln). Daher schlage ich vor, wir verwenden einfach zwei vor dem Roboter angebrachte Walzen, um Würfel die der Roboter vor sich herschiebt bei Wandkontakt nach oben umzuwälzen. Die Würfel sind dann zwar nicht sortiert, aber sie sind zumindest teilweise gestapelt und platzsparend in der Bauzone abgelegt.
- Die Wasserkugeln können ähnlich mit Hilfe einer größeren Variante unserer Raketenabschussvorrichtung von diesem Jahr mit nur einem Aktor in den Tank geschossen werden. Zum Ablegen in die Kläranlage kann eine Auslassklappe verwendet werden, welche die in einer Schale gesammelten Wasserkugeln direkt ins Klärbecken abläßt. Die Wasserspender werden mit einem kleinen starken Stab, welcher sich radial um eine vertikale Achse dreht entleert. Die Wasserkugeln sollen dann direkt in eine hoch gelegene weiche Schale fallen, die dann sowohl für den Transport zur Kläranlage, als auch zum Tank genutzt werden können.
- Da viele Aktoren und Sensoren auf der Vorderseite des Roboters angebracht werden, kann auf der Heckseite das Bedienpult und der Akku positioniert werden.
