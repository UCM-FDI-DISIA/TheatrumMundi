function changeCharacter(character) {
    const characters = {
      lucy: {
        name: "LUCY",
        img: "images/lucy.png",
        description: "Lucy, policía desde los 21 años, su pasión y dedicación son admirables. En los últimos meses, su vista ha comenzado a fallar. Los médicos le advierten que la perderá por completo, pero Lucy no está lista para aceptarlo. Los grandes cambios no son fáciles de asumir."
      },
      keisara: {
        name: "KEISARA",
        img: "images/keisara.png",
        description: "Keisara (Kei para los amigos) es una escritora, amante de las mariposas, las flores y exprimir cada segundo del día. Siempre ha sentido que no hay suficiente tiempo en la vida y con su trigésimo cumpleaños acercándose, la sensación solo crece."
      },
      sol: {
        name: "SOL",
        img: "images/sol.png",
        description: "Sol, un programador que siempre quiso dedicarse a la música. Ha sido el mejor amigo de Keisara desde que son niños y admira como ella se ha dedicado a lo que realmente quería. Sin embargo, siente que si él no actúa como los demás quieren, el mundo le dejará atrás."
      }
    };
    
    document.getElementById("character-name").innerText = characters[character].name;
    document.getElementById("character-image").src = characters[character].img;
    document.getElementById("character-description").innerText = characters[character].description;
  }