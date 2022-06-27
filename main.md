\mainpage %Service CSV

Ce service permet de créer un tableau 2D de valeur avec des lignes et des colonnes nommées. Aujourd'hui, le format de fichier en sortie est le format CSV.
Ce service peut être utilisé comme service classique à définir dans l'AXL d'un module ou comme singleton pour avoir une instance unique pour tous les modules.

Ce service se veut simple d'utilisation mais permet aussi une utilisation plus avancée.
Un premier exemple d'utilisation simple pourrait être celui de l'exemple 1 (TableExample1) :
On utilise le service en tant que singleton. Sur un projet faisant plusieurs boucles.

Lors de l'init :
- on récupère un pointeur vers ce singleton, 
- on appelle la méthode init() du service.

Lors de la boucle :
- on crée une colonne "Iteration 1",
- on ajoute plusieurs lignes : "Variable 1", "Variable 2", &c.

Lors de l'exit :
- on appelle la méthode print() et/ou la méthode writeFile().
