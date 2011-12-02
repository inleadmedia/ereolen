<?php
/**
 * @file
 *
 * Popular loaned page pager template.
 */
?>

<div id="pager">
  <?php if ($data['pages'] > 1) : ?>
  <div class="navwide"><a class="first" href="#first">&lt;&lt;</a></div>
  <div class="navwide"><a class="prev" href="#prev">&lt;</a></div>
  <?php for ($i = 1; $i <= $data['pages']; $i++) : ?>
  <div class="navwide nav-placeholder"><a class="page" href="#<?php print $i; ?>"><?php print ($i == 1) ? '[' . $i . ']' : $i; ?></a></div>
  <?php ;endfor ?>
  <div class="navwide"><a class="next" href="#next">&gt;</a></div>
  <div class="navwide"><a class="last" href="#last">&gt;&gt;</a></div>
  <?php ;endif ?>
</div>
