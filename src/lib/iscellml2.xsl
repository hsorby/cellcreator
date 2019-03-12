<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
        xmlns:mathml="http://www.w3.org/1998/Math/MathML"
        xmlns:cellml20="http://www.cellml.org/cellml/2.0#"
        xmlns:cmeta="http://www.cellml.org/metadata/1.0#"
        xmlns:xlink="http://www.w3.org/1999/xlink"
        exclude-result-prefixes="cmeta mathml">

  <xsl:output method="xml" version="2.0" encoding="UTF-8" indent="no"/>

  <!-- Any elements or attributes not specified below will be dropped -->
  <xsl:template match="* | @*" priority="-1.0">
  </xsl:template>

  <!-- We treat the root element specially so the cellml prefix gets declared globally in the result document -->
  <xsl:template match="cellml20:model">
    <model xmlns="http://www.cellml.org/cellml/2.0#" xmlns:cellml="http://www.cellml.org/cellml/2.0#">
      <xsl:copy-of select="@name"/>
    </model>
  </xsl:template>

</xsl:stylesheet>

