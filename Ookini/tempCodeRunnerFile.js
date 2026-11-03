function puedeReservar(cantidad, tazasVendidas) {
  if (cantidad <= 2 && (tazasVendidas + cantidad) <= 50) {
    console.log("Reserva confirmada");
  } else {
    console.log("No se puede realizar la reserva");
  }
}